#include "SymbolTableVisitor.h"



std::any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    //scope global de main géré par visitBlock
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
    functionReturnTypes[fctName] = returnType;

    // Ne PAS utiliser visitChildren : gérer le scope manuellement
    pushScope();

    for (auto stmt : ctx->stmt()) {
        this->visit(stmt);
    }

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

std::any SymbolTableVisitor::visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx) {
    std::string fctName = ctx->ID()->getText();

    if (functionReturnTypes.count(fctName) && functionReturnTypes[fctName] == VOID) {
        std::cerr << "Erreur : la fonction void '" << fctName << "' ne peut pas être utilisée dans une expression.\n";
        errorFlag = true;
    } else if (!functionReturnTypes.count(fctName)) {
        std::cerr << "Erreur : fonction '" << fctName << "' appelée mais non déclarée.\n";
        errorFlag = true;
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

