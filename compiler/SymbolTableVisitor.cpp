#include "SymbolTableVisitor.h"



std::any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    //scope global de main géré par visitBlock
    // Fonctions builtin de la bibliothèque standard
    functions["getchar"] = {INT32, 0};
    functions["putchar"] = {INT32, 1}; // putchar retourne int en C
    return visitChildren(ctx);
}



std::any SymbolTableVisitor::visitBlock(ifccParser::BlockContext *ctx) {
    pushScope();
    auto result = visitChildren(ctx);
    
    for (auto& varName : scopeStack.back()) {
        if (!usedVars.count(varName)) {
            std::cerr << "Avertissement : variable '" << varName 
                      << "' déclarée mais jamais utilisée.\n";
        }
    }
    
    popScope();
    return result;
}

std::any SymbolTableVisitor::visitDeclar(ifccParser::DeclarContext *ctx) {
    for (size_t i = 0; i < ctx->ID().size(); i++) {
        std::string varName = ctx->ID(i)->getText();

        if (isDeclaredInCurrentScope(varName)) {
            std::cerr << "Erreur : variable '" << varName 
                      << "' déjà déclarée dans ce bloc.\n";
            errorFlag = true;
        } else {
            declare(varName);
        }
    }
    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitFonctDecl(ifccParser::FonctDeclContext *ctx) {
    
    std::string fctName = ctx->ID()->getText();
    // recurerer le type de retour preciser avant le nom de la fonction (void ou int)
    Type returnType = (ctx->getStart()->getText() == "void") ? VOID : INT32; 
    int paramCount = ctx->list_decl_param() ? ctx->list_decl_param()->ID().size() : 0;

    if (paramCount > 6) {
        std::cerr << "Erreur : fonction '" << fctName 
                  << "' a plus de 6 paramètres, ce qui n'est pas supporté.\n";
        errorFlag = true;
    }
    functions[fctName] = {returnType, paramCount};
    // Ne PAS utiliser visitChildren : gérer le scope manuellement
    pushScope();

    for (auto id : ctx->list_decl_param()->ID()) {
        declare(id->getText());
    }

    this->visit(ctx->block());

    // Vérification des variables non utilisées dans la fonction
    for (auto& varName : scopeStack.back()) {
        if (!usedVars.count(varName)) {
            std::cerr << "Avertissement : variable '" << varName
                      << "' déclarée mais jamais utilisée.\n";
        }
    }

    popScope();
    return 0;
}



void SymbolTableVisitor::checkFunctionCall(const std::string& fctName, 
                                            int argCount, bool usedInExpr) {
    if (!functions.count(fctName)) {
        std::cerr << "Erreur : fonction '" << fctName << "' appelée mais non déclarée.\n";
        errorFlag = true;
        return;
    }
    if (usedInExpr && functions[fctName].returnType == VOID) {
        std::cerr << "Erreur : la fonction void '" << fctName 
                  << "' ne peut pas être utilisée dans une expression.\n";
        errorFlag = true;
    }
    if (argCount != functions[fctName].paramCount) {
        std::cerr << "Erreur : fonction '" << fctName << "' appelée avec " 
                  << argCount << " argument(s), attendu " 
                  << functions[fctName].paramCount << ".\n";
        errorFlag = true;
    }
}

std::any SymbolTableVisitor::visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx) {
    std::string fctName = ctx->ID()->getText();
    int argCount = ctx->list_param() ? ctx->list_param()->expr().size() : 0;
    checkFunctionCall(fctName, argCount, true);

    //visiter les arguments éventuels pour detecter var non déclarées
    if (ctx->list_param()) {
        for (auto expr : ctx->list_param()->expr()) {
            this->visit(expr);
        }
    }

    return 0;
}
std::any SymbolTableVisitor::visitAssign(ifccParser::AssignContext *ctx) {
    std::string varName = ctx->ID()->getText();

    if (!isDeclared(varName)) {
        std::cerr << "Erreur : variable '" << varName 
                  << "' utilisée sans déclaration.\n";
        errorFlag = true;
    } else {
        usedVars.insert(varName);
    }
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitExprId(ifccParser::ExprIdContext *ctx) {
    if (ctx->ID() != nullptr) {
        std::string varName = ctx->ID()->getText();
        if (!isDeclared(varName)) {
            std::cerr << "Erreur : variable '" << varName 
                      << "' utilisée sans déclaration.\n";
            errorFlag = true;
        } else {
            usedVars.insert(varName);
        }
    }
    return visitChildren(ctx);
}

