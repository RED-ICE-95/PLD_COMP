#include "IR.h"
#include <algorithm>
using namespace std;


int CFG::nextBBnumber = 0;

void CFG::push_scope() {}  
void CFG::pop_scope()  {}  

void CFG::add_to_symbol_table(string name, Type t, int arraySize, bool isPointer) {
    SymbolType[name] = t;
    isArrayMap[name] = (arraySize > 0);
    isPointerMap[name] = isPointer;

    if (arraySize > 0) {
        nextFreeSymbolIndex += (arraySize * 4);
        SymbolIndex[name] = nextFreeSymbolIndex;
    } else if (isPointer) {
        nextFreeSymbolIndex += 8;
        SymbolIndex[name] = nextFreeSymbolIndex;
    } else {
        nextFreeSymbolIndex += 4;
        SymbolIndex[name] = nextFreeSymbolIndex;
    }
}

int CFG::get_var_index(string name) {
    if (SymbolIndex.count(name))
        return SymbolIndex[name];
    return -1;
}

Type CFG::get_var_type(string name) {
    if (SymbolType.count(name))
        return SymbolType[name];
    return INT32;
}


void CFG::add_bb(BasicBlock* bb) {
    bbs.push_back(bb);
    current_bb = bb;
}


string CFG::create_new_tempvar(Type t, bool isPointer) {
    string name = "!tmp" + to_string(nextFreeSymbolIndex);
    add_to_symbol_table(name, t, 0, isPointer);
    return name;
}


string CFG::new_BB_name() {
    return "BB" + to_string(nextBBnumber++);
}


string CFG::IR_reg_to_asm(string reg) {
    return "-" + to_string(get_var_index(reg)) + "(%rbp)";
}


void CFG::gen_asm_header(ostream& o) {
    o << "  .text\n";
    o << "  .globl " << ast->name << "\n";
    o << ast->name << ":\n";
}


void CFG::gen_asm_prologue(ostream& o) {
    o << "  pushq %rbp\n";
    o << "  movq %rsp, %rbp\n";
    int rawSize = nextFreeSymbolIndex - 4;
    int stackSize = ((rawSize + 15) / 16) * 16;
    if (stackSize > 0) {
        o << "  subq $" << stackSize << ", %rsp\n";
    }
}


void CFG::gen_asm_epilogue(ostream& o) {
    if (ast->returnType != VOID) {
    o << "  movl " << IR_reg_to_asm("!ret") << ", %eax\n";
    }
    o << "  movq %rbp, %rsp\n";
    o << "  popq %rbp\n";
    o << "  ret\n";
}


void CFG::gen_asm(ostream& o) {
    gen_asm_header(o);
    gen_asm_prologue(o);
    for (BasicBlock* bb : bbs)
        bb->gen_asm(o);
    exit_bb->gen_asm(o);  
}




void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
    instrs.push_back(new IRInstr (this, op, t, params));
}


void BasicBlock::gen_asm(ostream& o) {
    o << label << ":\n";
    for (IRInstr* instr : instrs)
        instr->gen_asm(o);


    if (exit_true == nullptr) {
        cfg->gen_asm_epilogue(o);
    } else if (exit_false == nullptr) {
        o << "  jmp " << exit_true->label << "\n";
    } else {
        o << "  cmpl $0, " << cfg->IR_reg_to_asm(test_var_name) << "\n";
        o << "  je "  << exit_false->label << "\n";
        o << "  jmp " << exit_true->label  << "\n";
    }
}


void IRInstr::gen_asm(ostream& o) {
    switch(op) {
        case ldconst:
            o << "  movl $" << params[1] << ", " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case copy:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case add:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  addl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case sub:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  subl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case mul:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  imull " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_eq:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cmpl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  sete %al\n";
            o << "  movzbl %al, %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_lt:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cmpl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  setl %al\n";
            o << "  movzbl %al, %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_le:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cmpl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  setle %al\n";
            o << "  movzbl %al, %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case call:
        {
            
            o << "  movl $0, %eax\n";
            o << "  call " << params[0] << "@PLT\n";
            if (params[1] != "")
                o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[1]) << "\n";
            break;
        }
        case div:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cdq\n";
            o << "  idivl " << bb->cfg->IR_reg_to_asm(params[2]) << "\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case mod:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cdq\n";
            o << "  idivl " << bb->cfg->IR_reg_to_asm(params[2]) << "\n";
            o << "  movl %edx, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case bit_or:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  orl "  << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case bit_xor:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  xorl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case bit_and:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  andl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case unary_minus:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  negl %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case unary_not:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cmpl $0, %eax\n";
            o << "  sete %al\n";
            o << "  movzbl %al, %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_ne:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cmpl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  setne %al\n";
            o << "  movzbl %al, %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_gt:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cmpl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  setg %al\n";
            o << "  movzbl %al, %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_ge:
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %eax\n";
            o << "  cmpl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %eax\n";
            o << "  setge %al\n";
            o << "  movzbl %al, %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case copy_from_reg:
            // params[0] = dest var on stack, params[1] = source register
            o << "  movl " << params[1] << ", " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case copy_to_reg:
            // params[0] = dest register, params[1] = source var on stack
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", " << params[0] << "\n";
            break;
        case copy_ptr:
            o << "  movq " << bb->cfg->IR_reg_to_asm(params[1]) << ", %rax\n";
            o << "  movq %rax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case copy_ptr_from_reg:
            o << "  movq " << params[1] << ", " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case copy_ptr_to_reg:
            o << "  movq " << bb->cfg->IR_reg_to_asm(params[1]) << ", " << params[0] << "\n";
            break;
        case push_arg:
            // pushq étend à 64-bit pour respecter l'alignement
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[0]) << ", %eax\n";
            o << "  pushq %rax\n";
            break;
        case load_param:
            // params[1] = offset depuis %rbp (ex: "16")
            o << "  movl " << params[1] << "(%rbp), %eax\n";
            o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case stack_cleanup:
        {
            // params[0] = bytes to clean from stack
            int bytes = stoi(params[0]);
            if (bytes > 0)
                o << "  addq $" << bytes << ", %rsp\n";
            else if (bytes < 0)
                o << "  subq $" << (-bytes) << ", %rsp\n";
            break;
        }
        case rmem:
            o << "  movslq " << bb->cfg->IR_reg_to_asm(params[2]) << ", %rcx\n";
            if (bb->cfg->is_pointer(params[1])) {
                o << "  movq " << bb->cfg->IR_reg_to_asm(params[1]) << ", %rax\n";
            } else {
                o << "  leaq " << bb->cfg->IR_reg_to_asm(params[1]) << ", %rax\n";
            }
            o << "  movl (%rax,%rcx,4), %edx\n";
            o << "  movl %edx, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
            
        case wmem:
            {
                o << "  movslq " << bb->cfg->IR_reg_to_asm(params[1]) << ", %rcx\n";
                if (bb->cfg->is_pointer(params[0])) {
                    o << "  movq " << bb->cfg->IR_reg_to_asm(params[0]) << ", %rax\n";
                } else {
                    o << "  leaq " << bb->cfg->IR_reg_to_asm(params[0]) << ", %rax\n";
                }
                bool is_const = !params[2].empty() && std::all_of(params[2].begin(), params[2].end(), [](char c){ return (c == '-' || isdigit(c)); });
                if (is_const) {
                    o << "  movl $" << params[2] << ", %edx\n";
                } else {
                    o << "  movl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %edx\n";
                }
                o << "  movl %edx, (%rax,%rcx,4)\n";
            }
            break;
        case address_of:
            o << "  leaq " << bb->cfg->IR_reg_to_asm(params[1]) << ", %rax\n";
            o << "  movq %rax, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        default:
            break;
    }
}


void BasicBlock::gen_asm_msp430(ostream& o) {
    o << label << ":\n";
    for (IRInstr* instr : instrs)
        instr->gen_asm_msp430(o);


    if (exit_true == nullptr) {
        cfg->gen_asm_epilogue(o);
    } else if (exit_false == nullptr) {
        o << "  jmp " << exit_true->label << "\n";
    } else {
        o << "  cmp #0, " << cfg->IR_reg_to_asm(test_var_name) << "\n";
        o << "  jeq " << exit_false->label << "\n";
        o << "  jmp " << exit_true->label << "\n";
    }
}


void IRInstr::gen_asm_msp430(ostream& o) {
    switch(op) {
        case ldconst:
            o << "  mov #" << params[1] << ", " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case copy_ptr:
        case copy:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case add:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  add " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case sub:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  sub " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case mul:
            cerr << "Erreur : multiplication non supportée sur MSP430\n";
            exit(1);
            break;
        case div:
            cerr << "Erreur : division non supportée sur MSP430\n";
            exit(1);
            break;
        case mod:
            cerr << "Erreur : modulo non supporté sur MSP430\n";
            exit(1);
            break;
        case cmp_eq:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  cmp " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov #1, R15\n";   // 1 par défaut
            o << "  jeq $+4\n";       // saute si égal → R15 reste 1
            o << "  mov #0, R15\n";   // R15=0 si pas égal
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_ne:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  cmp " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov #1, R15\n";
            o << "  jne $+4\n";
            o << "  mov #0, R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_lt:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  cmp " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov #0, R15\n";
            o << "  jge $+4\n";
            o << "  mov #1, R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_le:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  cmp " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov #0, R15\n";
            o << "  jl $+4\n";       // jl = jump if less (signé)
            o << "  mov #1, R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_gt:
            // a > b  devient  b < a, on inverse les opérandes
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  cmp " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  mov #0, R15\n";
            o << "  jge $+4\n";
            o << "  mov #1, R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case cmp_ge:
            // idem, a >= b  devient  b <= a
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  cmp " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  mov #0, R15\n";
            o << "  jl $+4\n";
            o << "  mov #1, R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case bit_or:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  bis " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case bit_and:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  and " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case bit_xor:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  xor " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case unary_minus:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  inv R15\n";  // inversion bit à bit
            o << "  inc R15\n";  // +1 = complément à 2
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case unary_not:
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n";
            o << "  cmp #0, R15\n";
            o << "  mov #0, R15\n";
            o << "  jeq $+4\n";
            o << "  mov #1, R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case call:
        {
            // 1. Placer les arguments dans les registres MSP430 (R12, R13, R14)
            std::vector<std::string> mspRegs = {"R12", "R13", "R14"};
            for(size_t i = 2; i < params.size() && (i - 2) < mspRegs.size(); i++) {
                o << "  mov " << bb->cfg->IR_reg_to_asm(params[i]) << ", " << mspRegs[i-2] << "\n";
            }
            
            // 2. Appel de la fonction (syntaxe MSP430)
            o << "  call #" << params[0] << "\n";
            
            // 3. Récupération du résultat depuis R15
            if (params[1] != "")
                o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[1]) << "\n";
            break;
        }
        case copy_from_reg:
        case copy_ptr_from_reg:
            // params[0] = variable destination sur la pile, params[1] = registre source (R12, R13, R14)
            o << "  mov " << params[1] << ", " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;

        case copy_to_reg:
        case copy_ptr_to_reg:
            // params[1] = variable source sur la pile, params[0] = registre destination (R12, R13, R14)
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", " << params[0] << "\n";
            break;
        case rmem:
            // params: destVar, baseVar, indexVar
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n"; // Charge l'index
            o << "  rla R15\n";                                             // Multiplie par 2
            
            if (bb->cfg->is_pointer(params[1])) {
                o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R14\n"; // Adresse directe (depuis un paramètre)
            } else {
                o << "  mov R4, R14\n";                                          // Récupère le Frame Pointer
                o << "  sub #" << bb->cfg->get_var_index(params[1]) << ", R14\n"; // Adresse de base locale
            }
            o << "  add R15, R14\n";                                        // Ajoute l'offset de l'index
            o << "  mov 0(R14), " << bb->cfg->IR_reg_to_asm(params[0]) << "\n"; // Lit vers la destination
            break;


        case wmem:
            // params: baseVar, indexVar, srcVar
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n"; 
            o << "  rla R15\n";                                           
            
            if (bb->cfg->is_pointer(params[0])) {
                o << "  mov " << bb->cfg->IR_reg_to_asm(params[0]) << ", R14\n"; 
            } else {
                o << "  mov R4, R14\n";                                          
                o << "  sub #" << bb->cfg->get_var_index(params[0]) << ", R14\n"; 
            }
            o << "  add R15, R14\n";                                       
            {
                // Vérifie si la source est une constante (ex: $5) ou une variable
                bool is_const = !params[2].empty() && std::all_of(params[2].begin(), params[2].end(), [](char c){ return (c == '-' || isdigit(c)); });
                if (is_const) {
                    o << "  mov #" << params[2] << ", 0(R14)\n";
                } else {
                    o << "  mov " << bb->cfg->IR_reg_to_asm(params[2]) << ", 0(R14)\n";
                }
            }
            break;

        case address_of:
            o << "  mov R4, R15\n"; 
            o << "  sub #" << bb->cfg->get_var_index(params[1]) << ", R15\n";
            o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        default:
            cerr << "Erreur : opération non supportée sur MSP430\n";
            exit(1);
    }
}

