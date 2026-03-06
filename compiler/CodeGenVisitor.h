#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include <iostream>
#include <unordered_map>
#include <string>

class  CodeGenVisitor : public ifccBaseVisitor {
	public:
        explicit CodeGenVisitor(std::unordered_map<std::string, int> offsets);

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDeclar(ifccParser::DeclarContext *ctx) override;
        virtual antlrcpp::Any visitAssign(ifccParser::AssignContext *ctx) override;

        // pour les différentes formes d'expressions
        virtual antlrcpp::Any visitExprConst(ifccParser::ExprConstContext *ctx) override;
        virtual antlrcpp::Any visitExprCharConst(ifccParser::ExprCharConstContext *ctx) override;
        virtual antlrcpp::Any visitExprId(ifccParser::ExprIdContext *ctx) override;
        virtual antlrcpp::Any visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *ctx) override;
        virtual antlrcpp::Any visitExprUnaryNot(ifccParser::ExprUnaryNotContext *ctx) override;
        virtual antlrcpp::Any visitExprParen(ifccParser::ExprParenContext *ctx) override;
        virtual antlrcpp::Any visitExprAdd(ifccParser::ExprAddContext *ctx) override;
        virtual antlrcpp::Any visitExprMult(ifccParser::ExprMultContext *ctx) override;
        virtual antlrcpp::Any visitExprBitOr(ifccParser::ExprBitOrContext *ctx) override;
        virtual antlrcpp::Any visitExprBitXor(ifccParser::ExprBitXorContext *ctx) override;
        virtual antlrcpp::Any visitExprBitAnd(ifccParser::ExprBitAndContext *ctx) override;
        virtual antlrcpp::Any visitExprEq(ifccParser::ExprEqContext *ctx) override;
        virtual antlrcpp::Any visitExprCmp(ifccParser::ExprCmpContext *ctx) override;

        private:
        std::unordered_map<std::string, int> varOffsets;
        int currentOffset = 0;
        
        int allocTemp();
};

