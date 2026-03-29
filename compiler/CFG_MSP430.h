#pragma once
#include "IR.h"

class CFG_MSP430 : public CFG {
public:
    CFG_MSP430(DefFonction* ast) : CFG(ast) {
        // MSP430: variables sont 2 bytes (16-bit), donc nextFreeSymbolIndex commence à 2
        // (et pas 4 comme sur x86)
        nextFreeSymbolIndex = 2;
    }

    void gen_asm(ostream& o) override;
    void gen_asm_prologue(ostream& o) override;
    void gen_asm_epilogue(ostream& o) override;
    void add_to_symbol_table(string name, Type t, int arraySize = 0) override;
    string IR_reg_to_asm(string reg) override;
};