/**
 * @file IR.h
 * @brief Structures de l'IR, de la CFG et des blocs basiques.
 *
 * Ce fichier définit les instructions intermédiaires, les blocs de base et le
 * graphe de flux de contrôle utilisé par le backend du compilateur.
 */
#ifndef IR_H
#define IR_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <initializer_list>
#include <unordered_map>

// Declarations from the parser 
#include "type.h"

using namespace std;

class BasicBlock;
class CFG;
class DefFonction;

/**
 * @brief Description d'une fonction pour la génération de code.
 *
 * Contient le nom, les paramètres et le type de retour de la fonction.
 */
class DefFonction {
public:
	string name;
	vector<pair<string, Type>> params; // list of (param name, param type)
	Type returnType;
	DefFonction(string name, vector<pair<string, Type>> params, Type returnType)
		: name(name), params(params), returnType(returnType) {}
};

/**
 * @brief Instruction intermédiaire à trois opérandes.
 *
 * Chaque instruction contient un pointeur vers son bloc de base et un
 * ensemble de paramètres décrivant l'opération.
 */
class IRInstr {
 
	public:
		typedef enum { X86, MSP430, ARM } Target;

		typedef enum {
			ldconst, // charger une constante dans une variable
			copy,    // copier la valeur d'une variable dans une autre
			copy_ptr,          
            copy_ptr_from_reg,
            copy_ptr_to_reg,
			add,     // addition de deux valeurs
			sub,     // soustraction de deux valeurs
			mul,     // multiplication de deux valeurs
			rmem,    // lire une valeur depuis la mémoire
			wmem,    // écrire une valeur en mémoire
			call,    // appel d'une fonction
			cmp_eq,  // comparaison d'égalité (==)
			cmp_lt,  // comparaison inférieur strict (<)
			cmp_le,   // comparaison inférieur ou égal (<=)
			div,
			mod,
			bit_or,
			bit_xor,
			bit_and,
			unary_minus,
			unary_not,
			cmp_ne,
			cmp_gt,
			cmp_ge,
			copy_from_reg,   // params: {destVar, regName}  e.g. {"x_0", "%edi"}
			copy_to_reg,     // params: {regName, srcVar}   e.g. {"%edi", "x_0"}
			stack_cleanup,      // params: {bytes}            e.g. {"16"} pour nettoyer 16 octets de la pile après un call
			push_arg,          // params: {argVar}           e.g. {"x_7"} pour pousser x_7 en argument au-delà du 6ème
			load_param,
			address_of
		} Operation;
		BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
		vector<string> params; /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d, params;  for wmem and rmem: choose yourself */



		/**  constructor */
		IRInstr(BasicBlock* bb_, Operation op, Type t, vector<string> params)
		    : bb(bb_), op(op), t(t), params(params) {}
		
		/** Actual code generation */
		void gen_asm(ostream &o); /**< x86 assembly code generation for this IR instruction */
		void gen_asm_msp430(ostream& o); /**< MSP430 assembly code generation for this IR instruction */
		
	private:
		Operation op;
		Type t;
};



/**
 * @brief Bloc basique dans le graphe de contrôle de flux.
 *
 * Un BasicBlock contient une liste d'instructions IR et des connexions vers
 * ses successeurs vrai/faux pour la génération de sauts conditionnels.
 */
class BasicBlock {
 public:
	BasicBlock(CFG* cfg, string entry_label) : cfg(cfg), label(entry_label), exit_true(nullptr), exit_false(nullptr) {};
	
	void gen_asm(ostream &o); /**< x86 assembly code generation for this basic block (very simple) */
	void gen_asm_msp430(ostream &o); /**< MSP430 assembly code generation for this basic block (very simple) */

	void add_IRInstr(IRInstr::Operation op, Type t, vector<string> params);

	BasicBlock* exit_true;  /**< pointer to the next basic block, true branch. If nullptr, return from procedure */ 
	BasicBlock* exit_false; /**< pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */
	string label; /**< label of the BB, also will be the label in the generated code */
	CFG* cfg; /** < the CFG where this block belongs */
	vector<IRInstr*> instrs; /** < the instructions themselves. */
  	string test_var_name;  /** < when generating IR code for an if(expr) or while(expr) etc,
													 store here the name of the variable that holds the value of expr */
 protected:

 
};



/**
 * @brief Graphe de contrôle de flux et table des symboles associée.
 *
 * Le CFG gère les blocs de base, la génération d'assemblage et la résolution des
 * variables temporaires et locales pour une fonction donnée.
 */
class CFG {
 public:
	CFG(DefFonction* ast) : ast(ast), nextFreeSymbolIndex(4) {};

	DefFonction* ast; /**< The AST this CFG comes from */
	
	void add_bb(BasicBlock* bb); 

	// x86 code generation: could be encapsulated in a processor class in a retargetable compiler
	virtual void gen_asm(ostream& o);
	virtual string IR_reg_to_asm(string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
	virtual void gen_asm_header(ostream& o); /**< generates the assembly header for this function, including the label and the prologue */
	virtual void gen_asm_prologue(ostream& o);
	virtual void gen_asm_epilogue(ostream& o);

	// symbol table methods
	virtual void add_to_symbol_table(string name, Type t, int arraySize = 0, bool isPointer = false);
    string create_new_tempvar(Type t, bool isPointer = false);
	int get_var_index(string name);
	Type get_var_type(string name);

	
	int getNextIndex() const { return nextFreeSymbolIndex; }

	// basic block management
	string new_BB_name();
	BasicBlock* current_bb;
	BasicBlock* exit_bb;

	void push_scope();
	void pop_scope();

	std::unordered_map<std::string, bool> isArrayMap;
    bool is_array(std::string name) { return isArrayMap[name]; }

	std::unordered_map<std::string, bool> isPointerMap;
    bool is_pointer(std::string name) { return isPointerMap[name]; }


 protected:
 
 	map<string, Type> SymbolType;
	map<string, int>  SymbolIndex;
	int nextFreeSymbolIndex; /**< to allocate new symbols in the symbol table */
 	static int nextBBnumber; /**< just for naming */
 	
 	vector <BasicBlock*> bbs; /**< all the basic blocks of this CFG*/
};


#endif