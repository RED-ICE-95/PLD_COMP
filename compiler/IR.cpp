#include "IR.h"
#include <algorithm>
using namespace std;

int CFG::nextBBnumber = 0;

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
    instrs.push_back(new IRInstr (this, op, t, params));
}

void CFG::push_scope() {
    ScopeIndex.push_back({});
    ScopeType.push_back({});
}

void CFG::pop_scope() {
    ScopeIndex.pop_back();
    ScopeType.pop_back();
}


void CFG::add_bb(BasicBlock* bb) {
    bbs.push_back(bb);
    current_bb = bb;
}

void CFG::add_to_symbol_table(string name, Type t, int arraySize) {
    ScopeType.back()[name] = t;
    
    if (arraySize > 0) {
        // 1. Pare-chocs "Haut" pour absorber les débordements positifs (ex: a[10])
        nextFreeSymbolIndex += 32; 
        
        // 2. On réserve la vraie taille du tableau
        nextFreeSymbolIndex += (arraySize * 4);
        
        // 3. On fixe l'adresse de base a[0] (tout en bas du tableau)
        ScopeIndex.back()[name] = nextFreeSymbolIndex;
        
        // 4. Pare-chocs "Bas" pour absorber les potentiels débordements négatifs (ex: a[-1])
        nextFreeSymbolIndex += 32;
    } else {
        // Variable simple (on ne change rien)
        nextFreeSymbolIndex += 4;
        ScopeIndex.back()[name] = nextFreeSymbolIndex;
    }
}


string CFG::create_new_tempvar(Type t) {
    string name = "!tmp" + to_string(nextFreeSymbolIndex);
    add_to_symbol_table(name, t);
    return name;
}

int CFG::get_var_index(string name) {
    for (int i = ScopeIndex.size() - 1; i >= 0; i--)
        if (ScopeIndex[i].count(name))
            return ScopeIndex[i][name];
    return -1; // ne devrait pas arriver si SymbolTableVisitor a fait son travail
}

Type CFG::get_var_type(string name) {
    for (int i = ScopeType.size() - 1; i >= 0; i--)
        if (ScopeType[i].count(name))
            return ScopeType[i][name];
    return INT32;
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
            int stackArgs = (params.size() > 2) ? stoi(params[2]) : 0;
            bool needPadding = (stackArgs % 2 != 0);

            if (needPadding) {
                o << "  subq $8, %rsp\n"; // alignement de la pile à 16 octets
            }
            o << "  movl $0, %eax\n";
            o << "  call " << params[0] << "@PLT\n";
            if (params[1] != "")
                o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[1]) << "\n";
            
            if (needPadding) {
                o << "  addq $8, %rsp\n"; // restaurer la pile
            }
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
            // params[0] = bytes to clean from stack
            o << "  addq $" << params[0] << ", %rsp\n";
            break;
        case rmem:
            // params: destVar, baseVar, indexVar
            o << "  movl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %ecx\n";
            o << "  leaq " << bb->cfg->IR_reg_to_asm(params[1]) << ", %rax\n";
            o << "  movl (%rax,%rcx,4), %edx\n";
            o << "  movl %edx, " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case wmem:
            // params: baseVar, indexVar, srcVar
            {
                o << "  movl " << bb->cfg->IR_reg_to_asm(params[1]) << ", %ecx\n";
                o << "  leaq " << bb->cfg->IR_reg_to_asm(params[0]) << ", %rax\n";
                // Si params[2] est un nombre, on met un $ devant
                bool is_const = !params[2].empty() && std::all_of(params[2].begin(), params[2].end(), [](char c){ return (c == '-' || isdigit(c)); });
                if (is_const) {
                    o << "  movl $" << params[2] << ", %edx\n";
                } else {
                    o << "  movl " << bb->cfg->IR_reg_to_asm(params[2]) << ", %edx\n";
                }
                o << "  movl %edx, (%rax,%rcx,4)\n";
            }
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
            // convention d'appel MSP430 : args dans R12, R13, R14 (R15 pour le retour)
            for (int i = 2; i < (int)params.size(); i++) {
                if (i - 2 >= 3) {
                    cerr << "Erreur : MSP430 supporte max 3 arguments en registres\n";
                    exit(1);
                }
                string regs[] = {"R12", "R13", "R14"};
                o << "  mov " << bb->cfg->IR_reg_to_asm(params[i]) << ", " << regs[i-2] << "\n";
            }
            o << "  call #" << params[0] << "\n";
            if (params[1] != "")
                o << "  mov R15, " << bb->cfg->IR_reg_to_asm(params[1]) << "\n";
            break;
        case copy_from_reg:
            // params[0] = variable destination sur la pile
            // params[1] = registre source (R12, R13, R14)
            o << "  mov " << params[1] << ", " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        case rmem:
            // params: destVar, baseVar, indexVar
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[2]) << ", R15\n"; // Charge l'index
            o << "  rla R15\n";                                              // Multiplie par 2 (Rotate Left Arithmetically)
            o << "  mov R4, R14\n";                                          // Récupère le Frame Pointer
            o << "  sub #" << bb->cfg->get_var_index(params[1]) << ", R14\n";// Recule jusqu'à l'adresse de base du tableau
            o << "  add R15, R14\n";                                         // Ajoute l'offset de l'index
            o << "  mov 0(R14), " << bb->cfg->IR_reg_to_asm(params[0]) << "\n"; // Lit la mémoire vers la variable dest
            break;

        case wmem:
            // params: baseVar, indexVar, srcVar
            o << "  mov " << bb->cfg->IR_reg_to_asm(params[1]) << ", R15\n"; // Charge l'index
            o << "  rla R15\n";                                              // Multiplie par 2
            o << "  mov R4, R14\n";                                          // Récupère le Frame Pointer
            o << "  sub #" << bb->cfg->get_var_index(params[0]) << ", R14\n";// Recule jusqu'à l'adresse de base
            o << "  add R15, R14\n";                                         // Ajoute l'offset de l'index
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
        default:
            cerr << "Erreur : opération non supportée sur MSP430\n";
            exit(1);
    }
}