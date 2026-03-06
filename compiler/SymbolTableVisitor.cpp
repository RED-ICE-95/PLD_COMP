#include "SymbolTableVisitor.h"

std::any SymbolTableVisitor::visitDeclar(ifccParser::DeclarContext *ctx) {
    for (size_t i = 0; i < ctx->ID().size(); i++) {
        std::string varName = ctx->ID(i)->getText();

        if (varIndexMap.find(varName) != varIndexMap.end()) {
            std::cerr << "Erreur : variable '" << varName << "' déjà déclarée.\n";
            errorFlag = true;
        } else {
            varIndexMap[varName] = nextIndex;  // offset en multiples de 4
            std::cerr << "Déclaration : " << varName << " -> offset " << varIndexMap[varName] << "\n";
            nextIndex -= 4;  // incrémente pour le prochain int32
        }
    }

    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitAssign(ifccParser::AssignContext *ctx) {
    std::string varName = ctx->ID()->getText();

    if (varIndexMap.find(varName) == varIndexMap.end()) {
        std::cerr << "Erreur : variable '" << varName << "' utilisée sans déclaration.\n";
        errorFlag = true;
    } else {
        usedVars.insert(varName);
    }

    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitExprId(ifccParser::ExprIdContext *ctx) {
    if (ctx->ID() != nullptr) {
        std::string varName = ctx->ID()->getText();
        if (varIndexMap.find(varName) == varIndexMap.end()) {
            std::cerr << "Erreur : variable '" << varName  << "' utilisée sans déclaration.\n";
            errorFlag = true;
        } else {
            usedVars.insert(varName);
            std::cerr << "Usage : " << varName << " -> index " << varIndexMap[varName] << "\n";
        }
    }
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    auto result = visitChildren(ctx);

    // on vérifie si toutes les variables déclarées ont été utilisées
    for (auto &[varName, index] : varIndexMap) {
        if (usedVars.find(varName) == usedVars.end()) {
            std::cerr << "Avertissement : variable '" << varName 
                      << "' déclarée mais jamais utilisée.\n";
        }
    }

    return result;
}