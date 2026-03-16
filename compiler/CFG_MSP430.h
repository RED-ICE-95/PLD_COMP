#pragma once
#include "IR.h"

class CFG_MSP430 : public CFG {
public:
    CFG_MSP430(DefFonction* ast) : CFG(ast) {}

    void gen_asm(ostream& o) override;
    void gen_asm_prologue(ostream& o) override;
    void gen_asm_epilogue(ostream& o) override;
    void add_to_symbol_table(string name, Type t) override;
    string IR_reg_to_asm(string reg) override;
};