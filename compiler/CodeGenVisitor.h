#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "IR.h"
#include "CFG_MSP430.h"

#include <iostream>
#include <unordered_map>
#include <string>

class  CodeGenVisitor : public ifccBaseVisitor {
	public:
        explicit CodeGenVisitor(DefFonction* ast, IRInstr::Target target);

        virtual std::any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual std::any visitDeclar(ifccParser::DeclarContext *ctx) override;
        virtual std::any visitAssign(ifccParser::AssignContext *ctx) override;
        virtual std::any visitBlock(ifccParser::BlockContext *ctx) override;

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
        virtual std::any visitExprCall(ifccParser::ExprCallContext *ctx) override;
        virtual std::any visitFonctDecl(ifccParser::FonctDeclContext *ctx) override;
        virtual std::any visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx) override;


        private:
        CFG* cfg;
        IRInstr::Target target;  // Sauvegarde du target (x86 ou MSP430)
        vector<map<string, string>> scopeRename;

        string resolve(const string& name) {
                for (int i = scopeRename.size() - 1; i >= 0; i--)
                if (scopeRename[i].count(name))
                        return scopeRename[i][name];
                return name; // !ret, !tmpN passent tels quels
        }

        // ── Propagation de constantes ─────────────────────────────────────────
        // Convention : un visit* peut retourner "$n" (ex: "$42") pour signifier
        // une constante connue à la compilation. Aucune IRInstr n'est alors émise.
        // materialize() convertit une telle valeur en vrai registre IR si besoin.
        std::string materialize(const std::string& val);
};

