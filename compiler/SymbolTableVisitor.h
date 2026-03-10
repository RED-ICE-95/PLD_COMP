#pragma once

#include <string>
#include <unordered_set>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class SymbolTableVisitor : public ifccBaseVisitor {

public:
    virtual std::any visitDeclar(ifccParser::DeclarContext *ctx) override;
    virtual std::any visitAssign(ifccParser::AssignContext *ctx) override;
    virtual std::any visitExprId(ifccParser::ExprIdContext *ctx) override;
    virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
    
    bool hasErrors() const { return errorFlag; }

private:
    std::unordered_set<std::string> declaredVars;     // variables déclarées
    std::unordered_set<std::string> usedVars;         // variables utilisées
    bool errorFlag = false;
};