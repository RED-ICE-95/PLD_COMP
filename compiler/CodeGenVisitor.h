#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "IR.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include "generated/ifccParser.h"

class  CodeGenVisitor : public ifccBaseVisitor {
	public:
        explicit CodeGenVisitor(DefFonction* ast);

        virtual std::any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual std::any visitDeclar(ifccParser::DeclarContext *ctx) override;
        virtual std::any visitAssign(ifccParser::AssignContext *ctx) override;
        virtual std::any visitBlock(ifccParser::BlockContext *ctx) override;
        virtual std::any visitIfStmt(ifccParser::IfStmtContext *ctx) override;
        virtual std::any visitWhileStmt(ifccParser::WhileStmtContext *ctx) override;
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
        virtual std::any visitIncdec(ifccParser::IncdecContext *ctx) override;


        private:
        CFG* cfg;
        vector<map<string, string>> scopeRename;

        string resolve(const string& name) {
                for (int i = scopeRename.size() - 1; i >= 0; i--)
                if (scopeRename[i].count(name))
                        return scopeRename[i][name];
                return name; // !ret, !tmpN passent tels quels
        }
};