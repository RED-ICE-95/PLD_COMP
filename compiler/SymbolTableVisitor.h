
#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <iostream>

class SymbolTableVisitor : public ifccBaseVisitor {
public:
    SymbolTableVisitor() : nextIndex(0), errorFlag(false) {}

    // visiteurs
    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitDeclar(ifccParser::DeclarContext *ctx) override;
    virtual antlrcpp::Any visitAssign(ifccParser::AssignContext *ctx) override;
    virtual antlrcpp::Any visitExprId(ifccParser::ExprIdContext *ctx) override;
    virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;

    // lookup dans les scopes imbriqués
    bool lookup(std::string name, int &index);

    // indicateurs / info
    bool errorFlag;

private:
    std::vector<std::unordered_map<std::string,int>> scopes; // pile de scopes
    int nextIndex; // offset pour les variables
    std::set<std::string> usedVars; // variables utilisées
};