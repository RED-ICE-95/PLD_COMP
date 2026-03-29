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
    virtual std::any visitExprArrayAccess(ifccParser::ExprArrayAccessContext *ctx) override;
    virtual std::any visitAssignSimple(ifccParser::AssignSimpleContext *ctx) override;
    virtual std::any visitAssignArray(ifccParser::AssignArrayContext *ctx) override;
    virtual std::any visitAssignAdd(ifccParser::AssignAddContext *ctx) override;
    virtual std::any visitAssignSub(ifccParser::AssignSubContext *ctx) override;
    virtual std::any visitAssignMul(ifccParser::AssignMulContext *ctx) override;
    virtual std::any visitAssignDiv(ifccParser::AssignDivContext *ctx) override;
    virtual std::any visitAssignMod(ifccParser::AssignModContext *ctx) override;
    virtual std::any visitIncdec(ifccParser::IncdecContext *ctx) override;

    virtual std::any visitExprId(ifccParser::ExprIdContext *ctx) override;
    virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
    virtual std::any visitBlock(ifccParser::BlockContext *ctx) override;
    virtual std::any visitFonctDecl(ifccParser::FonctDeclContext *ctx) override;
    virtual std::any visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx) override;
    void checkFunctionCall(const std::string& fctName, int argCount, bool usedInExpr);
    bool hasErrors() const { return errorFlag; }


private:
    // pile de scopes
    std::vector<std::unordered_set<std::string>> scopeStack;
    std::unordered_set<std::string> usedVars;
    std::unordered_set<std::string> declaredVars;     // variables déclarées
    std::map<std::string, bool> isArrayVar;       // pour différencier les variables simples des tableaux (pour la vérification d'accès)    
    
    struct FunctionInfo {
        Type returnType;
        std::vector<Type> paramTypes;
    };
    std::map<std::string, FunctionInfo> functions;
   
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

    std::any checkVarUsed(const std::string& varName);
};