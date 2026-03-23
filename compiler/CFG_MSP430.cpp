#include "CFG_MSP430.h"
using namespace std;

void CFG_MSP430::add_to_symbol_table(string name, Type t) {
    ScopeType.back()[name] = t;
    ScopeIndex.back()[name] = nextFreeSymbolIndex;
    nextFreeSymbolIndex += 2; // 16 bits = 2 octets au lieu de 4
}

string CFG_MSP430::IR_reg_to_asm(string reg) {
    return "-" + to_string(get_var_index(reg)) + "(R4)";
    // R4 = frame pointer MSP430
}

void CFG_MSP430::gen_asm_prologue(ostream& o) {
    o << "  push R4\n";
    o << "  mov R1, R4\n";                      // R1=SP, R4=FP
    int rawSize = nextFreeSymbolIndex - 2;
    int stackSize = ((rawSize + 1) / 2) * 2;    // arrondir au multiple de 2
    if (stackSize > 0)
        o << "  sub #" << stackSize << ", R1\n";
}

void CFG_MSP430::gen_asm_epilogue(ostream& o) {
    o << "  mov " << IR_reg_to_asm("!ret") << ", R15\n"; // R15 = valeur de retour
    o << "  mov R4, R1\n";
    o << "  pop R4\n";
    o << "  ret\n";
}

void CFG_MSP430::gen_asm(ostream& o) {
    gen_asm_header(o);
    gen_asm_prologue(o);
    for (BasicBlock* bb : bbs)
        bb->gen_asm_msp430(o);
    exit_bb->gen_asm_msp430(o);
}