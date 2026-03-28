#include "SymbolTableVisitor.h"
using namespace std;


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
    // On itère sur les declItem (chaque ID avec son éventuelle initialisation)
    for (auto item : ctx->declItem()) {
        std::string varName = item->ID()->getText();

        //tableau ou variable simple ? (ex: int a; vs int a[10];)
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

    // visitChildren visite aussi les expr d'initialisation → visitExprId détecte les variables utilisées
    return visitChildren(ctx);
}

std::any SymbolTableVisitor::visitFonctDecl(ifccParser::FonctDeclContext *ctx) {
    
    std::string fctName = ctx->ID()->getText();
    // recuperer le type de retour preciser avant le nom de la fonction (void ou int)
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


    if (ctx->list_decl_param() != nullptr) {
        for (auto id : ctx->list_decl_param()->ID()) {
            declare(id->getText());
        }
    }
    // ----------------------------------------------------------------------

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

    // visiter les arguments éventuels pour detecter var non déclarées
    if (ctx->list_param()) {
        for (auto expr : ctx->list_param()->expr()) {
            // Si l'expression est juste un ID (ex: putchar(arr))
            if (auto idCtx = dynamic_cast<ifccParser::ExprIdContext*>(expr)) {
                std::string argName = idCtx->ID()->getText();
                if (isArrayVar[argName]) {
                    std::cerr << "Erreur sémantique : le tableau '" << argName 
                              << "' ne peut pas être passé en argument simple à la fonction '" 
                              << fctName << "'.\n";
                    errorFlag = true;
                }
            }
            this->visit(expr);
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
        cerr << "Erreur sémantique : le tableau '" << varName << "' ne peut pas être affecté comme une variable simple.\n";
        errorFlag = true;
    } else {
        usedVars.insert(varName);
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
        usedVars.insert(varName);
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
        } else if (isArrayVar[varName]) { // si c'est un tableau, on ne peut pas l'utiliser comme une variable simple (ex: a au lieu de a[0])
            std::cerr << "Erreur sémantique : le tableau '" << varName 
                      << "' ne peut pas être lu comme une variable simple (manque []).\n";
            errorFlag = true;
        } else {
            usedVars.insert(varName);
        }
    }
    return visitChildren(ctx);
}

    // helper commun pour tous les assign et incdec
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
        usedVars.insert(varName);
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

