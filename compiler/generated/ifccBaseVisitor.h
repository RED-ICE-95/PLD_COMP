
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ifccVisitor.h"


/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ifccBaseVisitor : public ifccVisitor {
public:

  virtual std::any visitAxiom(ifccParser::AxiomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProg(ifccParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmt(ifccParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssign(ifccParser::AssignContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclar(ifccParser::DeclarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(ifccParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprUnaryNot(ifccParser::ExprUnaryNotContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprEq(ifccParser::ExprEqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprMult(ifccParser::ExprMultContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprCmp(ifccParser::ExprCmpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprBitAnd(ifccParser::ExprBitAndContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprParen(ifccParser::ExprParenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprAdd(ifccParser::ExprAddContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprBitXor(ifccParser::ExprBitXorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprId(ifccParser::ExprIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprBitOr(ifccParser::ExprBitOrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprConst(ifccParser::ExprConstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprCharConst(ifccParser::ExprCharConstContext *ctx) override {
    return visitChildren(ctx);
  }


};

