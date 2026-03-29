#include "SymbolTableVisitor.h"



std::any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    //scope global de main géré par visitBlock
    // Fonctions builtin de la bibliothèque standard
    functions["getchar"] = {INT32, {}};
    functions["putchar"] = {INT32, {INT32}}; // putchar retourne int en C
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
    // On itère sur les declItem (chaque ID avec son éventuelle initialisation)
    for (auto item : ctx->declItem()) {
        std::string varName = item->ID()->getText();

        if (isDeclaredInCurrentScope(varName)) {
            std::cerr << "Erreur : variable '" << varName 
                      << "' déjà déclarée dans ce bloc.\n";
            errorFlag = true;
        } else {
            declare(varName);
        }
    }

    // visitChildren visite aussi les expr d'initialisation → visitExprId détecte les variables utilisées
    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitFonctDecl(ifccParser::FonctDeclContext *ctx) {
    
    std::string fctName = ctx->ID()->getText();
    Type returnType = (ctx->getStart()->getText() == "void") ? VOID : INT32;
    std::vector<Type> paramTypes;
    int paramCount = 0;
    if (ctx->list_decl_param()) {
        for (auto id : ctx->list_decl_param()->ID()) {
            // Pour l'instant, tous les paramètres sont int
            paramTypes.push_back(INT32);
            paramCount++;
        }
    }
    if (paramCount > 6) {
        std::cerr << "Erreur : fonction '" << fctName
                  << "' a plus de 6 paramètres, ce qui n'est pas supporté.\n";
        errorFlag = true;
    }
    functions[fctName] = {returnType, paramTypes};
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
    const FunctionInfo& info = functions[fctName];
    if (usedInExpr && info.returnType == VOID) {
        std::cerr << "Erreur : la fonction void '" << fctName
                  << "' ne peut pas être utilisée dans une expression.\n";
        errorFlag = true;
    }
    if (argCount != (int)info.paramTypes.size()) {
        std::cerr << "Erreur : fonction '" << fctName << "' appelée avec "
                  << argCount << " argument(s), attendu "
                  << info.paramTypes.size() << ".\n";
        errorFlag = true;
    }
    // Le type des arguments sera vérifié dans visitExprFonctCall
}

std::any SymbolTableVisitor::visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx) {
    std::string fctName = ctx->ID()->getText();
    int argCount = ctx->list_param() ? ctx->list_param()->expr().size() : 0;
    checkFunctionCall(fctName, argCount, true);

    // Vérification du type des arguments
    if (functions.count(fctName)) {
        const FunctionInfo& info = functions[fctName];
        for (int i = 0; i < argCount && i < (int)info.paramTypes.size(); ++i) {
            // Pour l'instant, on ne gère que int, mais on prépare pour d'autres types
            // Si on veut gérer char plus tard, il faudra analyser le type de chaque expr
            // Ici, on suppose que tout est int
            // (on pourrait ajouter une fonction getExprType(expr) si besoin)
            Type expected = info.paramTypes[i];
            Type actual = INT32; // TODO: déduire le type réel de l'expression
            if (expected != actual) {
                std::cerr << "Erreur : argument " << (i+1) << " de la fonction '" << fctName << "' : type incorrect.\n";
                errorFlag = true;
            }
        }
    }

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

