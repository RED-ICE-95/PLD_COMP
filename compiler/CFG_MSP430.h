/**
 * @file CFG_MSP430.h
 * @brief Extension de CFG pour la cible MSP430.
 */
#pragma once
#include "IR.h"

/**
 * @brief Spécialisation du graphe de contrôle de flux pour MSP430.
 *
 * Gère la génération d'assemblage et la gestion de la pile spécifique à
 * l'architecture MSP430.
 */
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