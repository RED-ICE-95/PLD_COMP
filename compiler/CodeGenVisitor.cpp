#include "CodeGenVisitor.h"
#include <unordered_map>
#include <string>

CodeGenVisitor::CodeGenVisitor(DefFonction* ast) {
    cfg = new CFG(ast);
    cfg->push_scope();
    
    // BB de sortie unique pour tous les return
    cfg->exit_bb = new BasicBlock(cfg, cfg->new_BB_name() + "_exit");
    
    BasicBlock* bb = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(bb);
    if (ast->returnType != VOID) {
        cfg->add_to_symbol_table("!ret", ast->returnType);
    }
}

std::any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    for (auto fctd : ctx->fonctDecl()) {
        this->visit(fctd);
    }
    this->visit(ctx->block());
    cfg->gen_asm(cout);
    return 0;
}

std::any CodeGenVisitor::visitFonctDecl(ifccParser::FonctDeclContext *ctx)
{
    static const vector<string> paramRegs = {
        "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"
    };

    string returnTypeText = ctx->getStart()->getText();
    Type returnType = (returnTypeText == "void") ? VOID : INT32;
    string fctName = ctx->ID()->getText();

    CFG* old_cfg = cfg;
    DefFonction* fctAst = new DefFonction(fctName, vector<pair<string, Type>>{}, returnType);
    cfg = new CFG(fctAst);
    cfg->push_scope();
    cfg->exit_bb = new BasicBlock(cfg, cfg->new_BB_name() + "_exit");
    BasicBlock* bb = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(bb);

    if (returnType != VOID) {
        cfg->add_to_symbol_table("!ret", returnType);
    }
    
    scopeRename.push_back({});
    // Allouer chaque paramètre formel et le copier depuis le registre
    auto paramIds = ctx->list_decl_param()->ID();
    for (size_t i = 0; i < paramIds.size(); i++) {
        string originalName = paramIds[i]->getText();
        string uniqueName = originalName + "_" + to_string(cfg->getNextIndex());
        cfg->add_to_symbol_table(uniqueName, INT32);
        scopeRename.back()[originalName] = uniqueName;  // ← après push_back ci-dessous
        // Copier le registre argument vers la variable locale
        cfg->current_bb->add_IRInstr(IRInstr::copy_from_reg, INT32, {uniqueName, paramRegs[i]});
    }

    
    // Re-enregistrer le mapping (push_back doit être avant la boucle — voir note)
    this->visit(ctx->block());
    scopeRename.pop_back();

    cfg->gen_asm(cout);
    cfg = old_cfg;
    return 0;
}

/*std::any CodeGenVisitor::visitList_decl_param(ifccParser::List_decl_paramContext *ctx)
{
    // on suppose que les fonctions ont 6 paramètres
    for (auto id : ctx->ID()) {
        string paramName = id->getText();
        Type paramType = (ctx->getStart()->getText() == "void") ? VOID : INT32; // on suppose que les paramètres sont du même type que le retour pour l'instant
        cfg->add_to_symbol_table(paramName, paramType);
        // si id = expr, on suppose que c'est un paramètre de type int
        //on verifie s'il existe un "= expr" pour le paramètre
        if (ctx->expr()) {
            string exprVar = any_cast<string>(this->visit(ctx->expr()));
            cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {paramName, exprVar});
            // attribuer la valeur de sortie de l'expression à la variable du paramètre
            //??

        }
         auto& instrs = cfg->current_bb->instrs; // si instrs est public
        if (!instrs.empty()) {
            IRInstr* last = instrs.back();
            if (last->params[0] == exprVar) {
                // Redirige la destination directement vers varName
                last->params[0] = varName;
                return varName;
            }
        }
        cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, exprVar});
        return varName;
    }
    return 0;
}*/



std::any CodeGenVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    scopeRename.push_back({});
    
    for (auto sctx : ctx->stmt())
        this->visit(sctx);
    scopeRename.pop_back();
    return 0;
}

std::any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{

    if (cfg->ast->returnType != VOID) {
        string exprVar = any_cast<string>(this->visit(ctx->expr()));
// Même optimisation que visitAssign
        auto& instrs = cfg->current_bb->instrs;
        if (!instrs.empty() && instrs.back()->params[0] == exprVar) {
            instrs.back()->params[0] = "!ret";
        } else {
            cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {"!ret", exprVar});
        }
    }

    // sauter vers le BB de sortie
    cfg->current_bb->exit_true = cfg->exit_bb;
    cfg->current_bb->exit_false = nullptr;
    
    // créer un nouveau BB mort pour les instructions qui suivent éventuellement
    BasicBlock* dead_bb = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(dead_bb);

    
    return 0;
}

std::any CodeGenVisitor::visitDeclar(ifccParser::DeclarContext *ctx)
{
    for (auto id : ctx->ID()) {
        string originalName = id->getText();
        string uniqueName = originalName + "_" + to_string(cfg->getNextIndex());
        cfg->add_to_symbol_table(uniqueName, INT32);
        scopeRename.back()[originalName] = uniqueName;
    }
    return 0;
}


std::any CodeGenVisitor::visitAssign(ifccParser::AssignContext *ctx)
{
    string varName = resolve(ctx->ID()->getText());
    string exprVar = any_cast<string>(this->visit(ctx->expr()));

    // Si exprVar est un tempvar frais, on peut juste le réutiliser
    // via un copy — mais pour éviter la redondance, on patche
    // directement la dernière instruction générée
    auto& instrs = cfg->current_bb->instrs; // si instrs est public
    if (!instrs.empty()) {
        IRInstr* last = instrs.back();
        if (last->params[0] == exprVar) {
            // Redirige la destination directement vers varName
            last->params[0] = varName;
            return varName;
        }
    }
    cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, exprVar});
    return varName;
}





std::any CodeGenVisitor::visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx)
{
    static const vector<string> paramRegs = {
        "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"
    };

    string fctName = ctx->ID()->getText();
    auto args = ctx->list_param()->expr();

    // Évaluer les arguments et les mettre dans les registres
    for (size_t i = 0; i < args.size(); i++) {
        string argVar = any_cast<string>(this->visit(args[i]));
        cfg->current_bb->add_IRInstr(IRInstr::copy_to_reg, INT32,
                                      {paramRegs[i], argVar});
    }

    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::call, INT32, {fctName, destVar});
    return destVar;
}

std::any CodeGenVisitor::visitExprConst(ifccParser::ExprConstContext *ctx)
{
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {destVar, ctx->CONST()->getText()});
    return destVar;
}

std::any CodeGenVisitor::visitExprCharConst(ifccParser::ExprCharConstContext *ctx)
{
    string text = ctx->CHAR_CONST()->getText(); 
    string inner = text.substr(1, text.size() - 2);    // on enlève guillemets
    int value = 0;
    if (inner.size() == 1) {    // caractère simple, le cast suffit (ex: 'Z')
        value = (int)(unsigned char)inner[0];
    } else if (inner.size() == 2 && inner[0] == '\\') { //  séquence d'échappement (ex: '\n')
        switch (inner[1]) {
            case 'n':  value = '\n'; break;
            case 't':  value = '\t'; break;
            case 'r':  value = '\r'; break;
            case '0':  value = '\0'; break;
            case '\\': value = '\\'; break;
            case '\'': value = '\''; break;
            case '"':  value = '"';  break;
            default:   value = (int)(unsigned char)inner[1]; break;
        }
    }
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {destVar, to_string(value)});
    return destVar;
}



std::any CodeGenVisitor::visitExprId(ifccParser::ExprIdContext *ctx)
{
    string varName = resolve(ctx->ID()->getText());
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {destVar, varName});
    return destVar;
}

std::any CodeGenVisitor::visitExprParen(ifccParser::ExprParenContext *ctx)
{
    return this->visit(ctx->expr());
}

// Opérations unaires
std::any CodeGenVisitor::visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *ctx)
{
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::unary_minus, INT32, {destVar, exprVar});
    return destVar;
}

std::any CodeGenVisitor::visitExprUnaryNot(ifccParser::ExprUnaryNotContext *ctx)
{
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::unary_not, INT32, {destVar, exprVar});
    return destVar;
}

// Opérations binaires
std::any CodeGenVisitor::visitExprAdd(ifccParser::ExprAddContext *ctx)
{
    string leftVar = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string destVar = cfg->create_new_tempvar(INT32);

    // instruction IR
    string opText = ctx->children[1]->getText();
    if (opText == "+") {
        cfg->current_bb->add_IRInstr(IRInstr::add, INT32, {destVar, leftVar, rightVar});
    } else {
        cfg->current_bb->add_IRInstr(IRInstr::sub, INT32, {destVar, leftVar, rightVar});
    }

    // on retourne la variable pour que le parent puisse l'utiliser
    return destVar;
}

std::any CodeGenVisitor::visitExprMult(ifccParser::ExprMultContext *ctx)
{
    string leftVar = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string destVar = cfg->create_new_tempvar(INT32);
    string opText = ctx->children[1]->getText();
    if (opText == "*") {
        cfg->current_bb->add_IRInstr(IRInstr::mul, INT32, {destVar, leftVar, rightVar});
    } else if (opText == "/") {
        cfg->current_bb->add_IRInstr(IRInstr::div, INT32, {destVar, leftVar, rightVar});
    } else { // %
        cfg->current_bb->add_IRInstr(IRInstr::mod, INT32, {destVar, leftVar, rightVar});
    }
    return destVar;
}

// Bit-à-bit
std::any CodeGenVisitor::visitExprBitOr(ifccParser::ExprBitOrContext *ctx)
{
    string leftVar = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::bit_or, INT32, {destVar, leftVar, rightVar});
    return destVar;
}

std::any CodeGenVisitor::visitExprBitXor(ifccParser::ExprBitXorContext *ctx)
{
    string leftVar = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::bit_xor, INT32, {destVar, leftVar, rightVar});
    return destVar;
}

std::any CodeGenVisitor::visitExprBitAnd(ifccParser::ExprBitAndContext *ctx)
{
    string leftVar = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::bit_and, INT32, {destVar, leftVar, rightVar});
    return destVar;
}

// Comparaisons
std::any CodeGenVisitor::visitExprEq(ifccParser::ExprEqContext *ctx)
{
    string leftVar = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string destVar = cfg->create_new_tempvar(INT32);
    string opText = ctx->children[1]->getText();
    if (opText == "==") {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, INT32, {destVar, leftVar, rightVar});
    } else { // !=
        cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, INT32, {destVar, leftVar, rightVar});
    }
    return destVar;
}

std::any CodeGenVisitor::visitExprCmp(ifccParser::ExprCmpContext *ctx)
{
    string leftVar = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string destVar = cfg->create_new_tempvar(INT32);
    string opText = ctx->children[1]->getText();
    if (opText == "<") {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_lt, INT32, {destVar, leftVar, rightVar});
    } else { // >
        cfg->current_bb->add_IRInstr(IRInstr::cmp_gt, INT32, {destVar, leftVar, rightVar});
    }
    return destVar;
}

// Appel de fonction en tant que statement (ex: putchar(x);)
std::any CodeGenVisitor::visitCall_stmt(ifccParser::Call_stmtContext *ctx)
{
    string functionName = ctx->ID()->getText();
    
    // Évaluer l'argument s'il existe
    vector<string> argVars;
    if (ctx->expr()) {
        string argVar = any_cast<string>(visit(ctx->expr()));
        argVars.push_back(argVar);
    }
    
    // Construire les paramètres de l'instruction IR call
    // Format: {nom_fonction, var_retour, arg1, arg2, ...}
    vector<string> callParams = {functionName, ""};  // "" = pas de retour utilisé
    callParams.insert(callParams.end(), argVars.begin(), argVars.end());
    
    cfg->current_bb->add_IRInstr(IRInstr::call, INT32, callParams);
    
    return 0;
}
