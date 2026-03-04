#pragma once

#include <string>
#include <unordered_map>
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

    // CodeGenVisitor pourra récupèrer cette map directement
    const std::unordered_map<std::string, int>& getVarOffsets() const { return varIndexMap; }

private:
    std::unordered_map<std::string, int> varIndexMap; // nom -> index
    std::unordered_set<std::string> usedVars;         // variables utilisées
    int nextIndex = 0;                                // compteur d'index
    bool errorFlag = false;
};