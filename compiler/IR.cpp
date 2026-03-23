#include "IR.h"
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

void CFG::add_to_symbol_table(string name, Type t) {
    ScopeType.back()[name] = t;
    ScopeIndex.back()[name] = nextFreeSymbolIndex;
    nextFreeSymbolIndex += 4;
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
            for (int i = 2; i < (int)params.size(); i++) {
                string regs[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
                o << "  movl " << bb->cfg->IR_reg_to_asm(params[i]) << ", " << regs[i-2] << "\n";
            }
            o << "  movl $0, %eax\n";
            o << "  call " << params[0] << "@PLT\n";
            if (params[1] != "")
                o << "  movl %eax, " << bb->cfg->IR_reg_to_asm(params[1]) << "\n";
            break;
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
            // params[0] est la variable destination sur la pile
            // params[1] correspond au registre source (R12, R13, R14)
            o << "  mov " << params[1] << ", " << bb->cfg->IR_reg_to_asm(params[0]) << "\n";
            break;
        default:
            cerr << "Erreur : opération non supportée sur MSP430\n";
            exit(1);
    }
}