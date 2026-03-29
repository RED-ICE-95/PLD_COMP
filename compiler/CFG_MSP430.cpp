#include "CFG_MSP430.h"
using namespace std;

void CFG_MSP430::add_to_symbol_table(string name, Type t, int arraySize, bool isPointer) {
    ScopeType.back()[name] = t;
    isArrayMap[name] = (arraySize > 0);
    isPointerMap[name] = isPointer; // Enregistre si c'est un pointeur (ex: paramètre de tableau)
    
    if (arraySize > 0) {
        // 1. Pare-chocs Haut (16 octets suffisent sur microcontrôleur)
        nextFreeSymbolIndex += 16; 
        
        // 2. Taille réelle du tableau (2 octets par int sur MSP430 !)
        nextFreeSymbolIndex += (arraySize * 2);
        
        // 3. Adresse de base a[0]
        ScopeIndex.back()[name] = nextFreeSymbolIndex;
        
        // 4. Pare-chocs Bas
        nextFreeSymbolIndex += 16;
    } else if (isPointer) {
        // Variable Pointeur sur MSP430 (adresse 16-bit = 2 octets)
        nextFreeSymbolIndex += 2;
        ScopeIndex.back()[name] = nextFreeSymbolIndex;
    } else {
        // Variable simple (2 octets)
        nextFreeSymbolIndex += 2;
        ScopeIndex.back()[name] = nextFreeSymbolIndex;
    }
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
    if (ast->returnType != VOID) {
    o << "  mov " << IR_reg_to_asm("!ret") << ", R15\n"; // R15 = valeur de retour
    }
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