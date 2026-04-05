/**
 * @file CodeGenVisitor.h
 * @brief Visiteur de génération de code pour la compilation.
 *
 * Ce fichier contient le visitor qui parcourt l'arbre syntaxique généré par
 * ANTLR et produit une représentation intermédiaire (IR) ainsi qu'une
 * CFG adaptée à la cible choisie.
 */
#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "IR.h"
#include "CFG_MSP430.h"

#include <iostream>
#include <unordered_map>
#include <string>

/**
 * @brief Visiteur de génération de code pour l'IR et la CFG.
 *
 * CodeGenVisitor visite les nœuds du parse tree et déclenche l'émission des
 * instructions intermédiaires et des blocs de contrôle de flux.
 */
class  CodeGenVisitor : public ifccBaseVisitor {
	public:
        /**
         * @brief Initialise le visiteur de génération de code.
         * @param ast    Description de la fonction courante.
         * @param target Cible d'assemblage (X86 ou MSP430).
         */
        explicit CodeGenVisitor(DefFonction* ast, IRInstr::Target target);

        virtual std::any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual std::any visitDeclar(ifccParser::DeclarContext *ctx) override;
        virtual std::any visitBlock(ifccParser::BlockContext *ctx) override;

        virtual std::any visitAssignSimple(ifccParser::AssignSimpleContext *ctx) override;
        virtual std::any visitAssignAdd(ifccParser::AssignAddContext *ctx) override;    
        virtual std::any visitAssignSub(ifccParser::AssignSubContext *ctx) override;
        virtual std::any visitAssignMul(ifccParser::AssignMulContext *ctx) override;
        virtual std::any visitAssignDiv(ifccParser::AssignDivContext *ctx) override;
        virtual std::any visitAssignMod(ifccParser::AssignModContext *ctx) override;
        virtual std::any visitAssignArray(ifccParser::AssignArrayContext *ctx) override;
        virtual std::any visitExprArrayAccess(ifccParser::ExprArrayAccessContext *ctx) override;
        

        // pour les différentes formes d'expressions
        virtual std::any visitExprConst(ifccParser::ExprConstContext *ctx) override;
        virtual std::any visitExprCharConst(ifccParser::ExprCharConstContext *ctx) override;
        virtual std::any visitExprId(ifccParser::ExprIdContext *ctx) override;
        virtual std::any visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *ctx) override;
        virtual std::any visitExprUnaryNot(ifccParser::ExprUnaryNotContext *ctx) override;
        virtual std::any visitExprParen(ifccParser::ExprParenContext *ctx) override;
        virtual std::any visitExprAdd(ifccParser::ExprAddContext *ctx) override;
        virtual std::any visitExprMult(ifccParser::ExprMultContext *ctx) override;
        virtual std::any visitExprBitOr(ifccParser::ExprBitOrContext *ctx) override;
        virtual std::any visitExprBitXor(ifccParser::ExprBitXorContext *ctx) override;
        virtual std::any visitExprBitAnd(ifccParser::ExprBitAndContext *ctx) override;
        virtual std::any visitExprEq(ifccParser::ExprEqContext *ctx) override;
        virtual std::any visitExprCmp(ifccParser::ExprCmpContext *ctx) override;
        virtual std::any visitCall_stmt(ifccParser::Call_stmtContext *ctx) override;
        virtual std::any visitFonctDecl(ifccParser::FonctDeclContext *ctx) override;
        virtual std::any visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx) override;

        virtual std::any visitIncdec(ifccParser::IncdecContext *ctx) override;
        virtual std::any visitExprAnd(ifccParser::ExprAndContext *ctx) override;
        virtual std::any visitExprOr(ifccParser::ExprOrContext *ctx) override;

        virtual std::any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
        virtual std::any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;

        virtual std::any visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) override;
        virtual std::any visitBreak_stmt(ifccParser::Break_stmtContext *ctx) override;
        virtual std::any visitContinue_stmt(ifccParser::Continue_stmtContext *ctx) override;


        private:
        CFG* cfg;
        BasicBlock* current_break_bb;
        BasicBlock* current_continue_bb;
        IRInstr::Target target;  // Sauvegarde du target (x86 ou MSP430)
        vector<map<string, string>> scopeRename;
        
        // Table des signatures de fonctions : nom -> nombre de paramètres
        std::unordered_map<std::string, int> functionSignatures;
        
        void initBuiltinFunctions() {
            functionSignatures["getchar"] = 0;
            functionSignatures["putchar"] = 1;
        }

        string resolve(const string& name) {
                for (int i = scopeRename.size() - 1; i >= 0; i--)
                if (scopeRename[i].count(name))
                        return scopeRename[i][name];
                return name; // !ret, !tmpN passent tels quels
        }

        // Propagation de constantes 
        // Convention : un visit* peut retourner "$n" (ex: "$42") pour signifier
        // une constante connue à la compilation. Aucune IRInstr n'est alors émise.
        // materialize() convertit une telle valeur en vrai registre IR si besoin.
        /**
         * @brief Matérialise une constante pliée en registre IR.
         *
         * Si la valeur est de la forme "$n", elle est chargée dans un
         * registre temporaire IR et ce registre est retourné.
         * @param val Valeur constante ou identifiant de registre.
         * @return Nom du registre IR contenant la valeur.
         */
        std::string materialize(const std::string& val);
};
