#include "SymbolTableVisitor.h"

std::any SymbolTableVisitor::visitDeclar(ifccParser::DeclarContext *ctx) {
    // On itère sur les declItem (chaque ID avec son éventuelle initialisation)
    for (auto item : ctx->declItem()) {
        std::string varName = item->ID()->getText();

        if (declaredVars.count(varName)) {
            std::cerr << "Erreur : variable '" << varName << "' déjà déclarée.\n";
            errorFlag = true;
        } else {
            declaredVars.insert(varName);
        }
    }

    // visitChildren visite aussi les expr d'initialisation → visitExprId détecte les variables utilisées
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitAssign(ifccParser::AssignContext *ctx) {
    std::string varName = ctx->ID()->getText();

    if (!declaredVars.count(varName)) {
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
        if (!declaredVars.count(varName)) {
            std::cerr << "Erreur : variable '" << varName << "' utilisée sans déclaration.\n";
            errorFlag = true;
        } else {
            usedVars.insert(varName);
        }
    }
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    auto result = visitChildren(ctx);

    for (auto &varName : declaredVars) {
        if (!usedVars.count(varName)) {
            std::cerr << "Avertissement : variable '" << varName << "' déclarée mais jamais utilisée.\n";
        }
    }

    return result;
}