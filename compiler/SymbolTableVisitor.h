#pragma once

#include <string>
#include <unordered_set>
#include <map>
#include "type.h"
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class SymbolTableVisitor : public ifccBaseVisitor {

public:
    virtual std::any visitDeclar(ifccParser::DeclarContext *ctx) override;
    virtual std::any visitAssign(ifccParser::AssignContext *ctx) override;
    virtual std::any visitExprId(ifccParser::ExprIdContext *ctx) override;
    virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
    virtual std::any visitBlock(ifccParser::BlockContext *ctx) override;
    virtual std::any visitFonctDecl(ifccParser::FonctDeclContext *ctx) override;
    virtual std::any visitExprCall(ifccParser::ExprCallContext *ctx) override;
    virtual std::any visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx) override;
    bool hasErrors() const { return errorFlag; }

private:
    // pile de scopes
    std::vector<std::unordered_set<std::string>> scopeStack;
    std::unordered_set<std::string> usedVars;
    std::unordered_set<std::string> declaredVars;     // variables déclarées
    
    std::map<std::string, Type> functionReturnTypes;     // variables assignées
    bool errorFlag = false;

    void pushScope() { scopeStack.push_back({}); }
    void popScope()  { scopeStack.pop_back(); }

    bool isDeclared(const std::string& name) {
        for (int i = scopeStack.size() - 1; i >= 0; i--)
            if (scopeStack[i].count(name)) return true;
        return false;
    }

    bool isDeclaredInCurrentScope(const std::string& name) {
        if (scopeStack.empty()) return false;
        return scopeStack.back().count(name) > 0;
    }

    void declare(const std::string& name) {
        scopeStack.back().insert(name);
    }
};