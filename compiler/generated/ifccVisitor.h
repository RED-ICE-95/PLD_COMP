
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ifccParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ifccParser.
 */
class  ifccVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ifccParser.
   */
    virtual std::any visitAxiom(ifccParser::AxiomContext *context) = 0;

    virtual std::any visitProg(ifccParser::ProgContext *context) = 0;

    virtual std::any visitStmt(ifccParser::StmtContext *context) = 0;

    virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *context) = 0;

    virtual std::any visitAssign(ifccParser::AssignContext *context) = 0;

    virtual std::any visitDeclar(ifccParser::DeclarContext *context) = 0;

    virtual std::any visitExprUnaryNot(ifccParser::ExprUnaryNotContext *context) = 0;

    virtual std::any visitExprEq(ifccParser::ExprEqContext *context) = 0;

    virtual std::any visitExprMult(ifccParser::ExprMultContext *context) = 0;

    virtual std::any visitExprCmp(ifccParser::ExprCmpContext *context) = 0;

    virtual std::any visitExprBitAnd(ifccParser::ExprBitAndContext *context) = 0;

    virtual std::any visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *context) = 0;

    virtual std::any visitExprParen(ifccParser::ExprParenContext *context) = 0;

    virtual std::any visitExprAdd(ifccParser::ExprAddContext *context) = 0;

    virtual std::any visitExprBitXor(ifccParser::ExprBitXorContext *context) = 0;

    virtual std::any visitExprId(ifccParser::ExprIdContext *context) = 0;

    virtual std::any visitExprBitOr(ifccParser::ExprBitOrContext *context) = 0;

    virtual std::any visitExprConst(ifccParser::ExprConstContext *context) = 0;

    virtual std::any visitExprCharConst(ifccParser::ExprCharConstContext *context) = 0;


};

