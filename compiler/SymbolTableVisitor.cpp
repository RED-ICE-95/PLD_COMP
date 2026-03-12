#include "SymbolTableVisitor.h"

std::any SymbolTableVisitor::visitDeclar(ifccParser::DeclarContext *ctx) {
    for (size_t i = 0; i < ctx->ID().size(); i++) {
        std::string varName = ctx->ID(i)->getText();

        if (declaredVars.count(varName)) {
            std::cerr << "Erreur : variable '" << varName << "' déjà déclarée.\n";
            errorFlag = true;
        } else {
            declaredVars.insert(varName);
        }
    }

    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitFonctDecl(ifccParser::FonctDeclContext *ctx) {
    std::string fctName = ctx->ID()->getText();
    functionReturnTypes[fctName] = VOID; // void hardcodé pour l'instant
    return visitChildren(ctx);
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