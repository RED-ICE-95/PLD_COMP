#include "SymbolTableVisitor.h"
using namespace std;

std::any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) {
    functions["getchar"] = {INT32, {}};
    functions["putchar"] = {INT32, {INT32}};  // putchar retourne int en C
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitBlock(ifccParser::BlockContext *ctx) {
    pushScope();
    auto result = visitChildren(ctx);

    for (auto& varName : scopeStack.back()) {
        if (!usedVarsStack.back().count(varName)) {
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

        bool isArray = (item->CONST() != nullptr && item->getText().find('[') != string::npos);
        isArrayVar[varName] = isArray;

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
    Type returnType = (ctx->getStart()->getText() == "void") ? VOID : INT32;
    std::vector<Type> paramTypes;
    if (ctx->list_decl_param()) {
        for (auto p : ctx->list_decl_param()->param()) {
            paramTypes.push_back(INT32);
        }
    }
    functions[fctName] = {returnType, paramTypes};
    

    pushScope();

    if (ctx->list_decl_param() != nullptr) {
        for (auto p : ctx->list_decl_param()->param()) { // On boucle sur param()
            string pName = p->ID()->getText(); // On extrait l'ID
            
            // On enregistre si le paramètre est un tableau (ex: int t[])
            bool isArray = p->getText().find('[') != string::npos;
            isArrayVar[pName] = isArray;

            declare(pName);
            markAsUsed(pName); // paramètres considérés comme utilisés
        }
    }

    this->visit(ctx->block());

    for (auto& varName : scopeStack.back()) {
        if (!usedVarsStack.back().count(varName)) {
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

    // Marquage des arguments comme utilisés (sans lever d'erreur si c'est un tableau)
    if (ctx->list_param()) {
        for (auto expr : ctx->list_param()->expr()) {
            if (auto idCtx = dynamic_cast<ifccParser::ExprIdContext*>(expr)) {
                std::string argName = idCtx->ID()->getText();
                markAsUsed(argName); // On le marque comme utilisé, c'est tout.
            } else {
                this->visit(expr); // On visite les autres expressions normales (calculs, etc.)
            }
        }
    }
    return 0;
}

std::any SymbolTableVisitor::visitAssignSimple(ifccParser::AssignSimpleContext *ctx) {
    string varName = ctx->ID()->getText();
    if (!isDeclared(varName)) {
        cerr << "Erreur : variable '" << varName << "' utilisée sans déclaration.\n";
        errorFlag = true;
    } else if (isArrayVar[varName]) {
        cerr << "Erreur sémantique : le tableau '" << varName
             << "' ne peut pas être affecté comme une variable simple.\n";
        errorFlag = true;
    } else {
        markAsUsed(varName);
    }
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitAssignArray(ifccParser::AssignArrayContext *ctx) {
    string varName = ctx->ID()->getText();
    if (!isDeclared(varName)) {
        cerr << "Erreur : tableau '" << varName << "' utilisé sans déclaration.\n";
        errorFlag = true;
    } else if (!isArrayVar[varName]) {
        cerr << "Erreur sémantique : '" << varName << "' n'est pas un tableau.\n";
        errorFlag = true;
    } else {
        markAsUsed(varName);
    }
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitExprArrayAccess(ifccParser::ExprArrayAccessContext *ctx) {
    string varName = ctx->ID()->getText();
    if (!isDeclared(varName)) {
        cerr << "Erreur : tableau '" << varName << "' utilisé sans déclaration.\n";
        errorFlag = true;
    } else if (!isArrayVar[varName]) {
        cerr << "Erreur sémantique : '" << varName << "' n'est pas un tableau.\n";
        errorFlag = true;
    } else {
        markAsUsed(varName);
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
            markAsUsed(varName);
        }
    }
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::checkVarUsed(const std::string& varName) {
    if (!isDeclared(varName)) {
        std::cerr << "Erreur : variable '" << varName
                  << "' utilisée sans déclaration.\n";
        errorFlag = true;
    } else if (isArrayVar[varName]) {
        std::cerr << "Erreur sémantique : '" << varName
                  << "' est un tableau, pas une variable simple.\n";
        errorFlag = true;
    } else {
        markAsUsed(varName);
    }
    return nullptr;
}

std::any SymbolTableVisitor::visitAssignAdd(ifccParser::AssignAddContext *ctx) {
    checkVarUsed(ctx->ID()->getText());
    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitAssignSub(ifccParser::AssignSubContext *ctx) {
    checkVarUsed(ctx->ID()->getText());
    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitAssignMul(ifccParser::AssignMulContext *ctx) {
    checkVarUsed(ctx->ID()->getText());
    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitAssignDiv(ifccParser::AssignDivContext *ctx) {
    checkVarUsed(ctx->ID()->getText());
    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitAssignMod(ifccParser::AssignModContext *ctx) {
    checkVarUsed(ctx->ID()->getText());
    return visitChildren(ctx);
}
std::any SymbolTableVisitor::visitIncdec(ifccParser::IncdecContext *ctx) {
    checkVarUsed(ctx->ID()->getText());
    return nullptr;
}