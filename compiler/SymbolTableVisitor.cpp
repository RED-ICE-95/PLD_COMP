#include "SymbolTableVisitor.h"

//ce visitor vérifie que les variables sont déclarées avant utilisation et qu'elles ne sont pas redéclarées dans le même scope
std::any SymbolTableVisitor::visitDeclar(ifccParser::DeclarContext *ctx) {

    auto &currentScope = scopes.back();

    for (size_t i = 0; i < ctx->ID().size(); i++) {

        std::string varName = ctx->ID(i)->getText();

        if (currentScope.find(varName) != currentScope.end()) {
            std::cerr << "Erreur : variable '" << varName << "' déjà déclarée dans ce bloc.\n";
            errorFlag = true;
        }
        else {
            currentScope[varName] = nextIndex;
            std::cerr << "Déclaration : " << varName << " -> offset " << nextIndex << "\n";

            nextIndex -= 4;
        }
    }

    return nullptr;
}

std::any SymbolTableVisitor::visitAssign(ifccParser::AssignContext *ctx) {

    std::string varName = ctx->ID()->getText();

    int index;

    if (!lookup(varName,index)) {
        std::cerr << "Erreur : variable '" << varName << "' utilisée sans déclaration.\n";
        errorFlag = true;
    }
    else {
        usedVars.insert(varName);
    }

    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitExprId(ifccParser::ExprIdContext *ctx) {

    std::string varName = ctx->ID()->getText();

    int index;

    if (!lookup(varName,index)) {
        std::cerr << "Erreur : variable '" << varName << "' utilisée sans déclaration.\n";
        errorFlag = true;
    }
    else {
        usedVars.insert(varName);
    }

    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    scopes.push_back({});   // on entre dans le scope global
    auto result = visitChildren(ctx);

    // on vérifie si toutes les variables déclarées ont été utilisées
    for (auto &scope : scopes) {
    for (auto &[varName, index] : scope) {
        if (usedVars.find(varName) == usedVars.end()) {
            std::cerr << "Avertissement : variable '" << varName 
                      << "' déclarée mais jamais utilisée.\n";
        }
    }
}

    return result;
}

std::any SymbolTableVisitor::visitBlock(ifccParser::BlockContext *ctx) {

    scopes.push_back({});   // nouveau scope

    visitChildren(ctx);

    scopes.pop_back();      // sortir du scope

    return nullptr;
}

// fonction de lookup pour vérifier si une variable est déclarée dans les scopes imbriqués (Elle cherche du scope interne vers externe.)
bool SymbolTableVisitor::lookup(std::string name, int &index) {

    for (int i = scopes.size()-1; i >= 0; i--) {

        auto it = scopes[i].find(name);

        if (it != scopes[i].end()) {
            index = it->second;
            return true;
        }
    }

    return false;
}