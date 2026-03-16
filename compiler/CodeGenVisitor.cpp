#include "CodeGenVisitor.h"
#include <unordered_map>
#include <string>

// ── Propagation de constantes ────────────────────────────────────────────
// Un visit* retourne "$n" quand la valeur est connue à la compilation.
// Cela évite d'émettre des IRInstr inutiles ; l'émission est différée.
static bool   isConst(const string& v)  { return !v.empty() && v[0] == '$'; }
static int    getConst(const string& v) { return stoi(v.substr(1)); }
static string makeConst(int v)          { return "$" + to_string(v); }
// ────────────────────────────────────────────────────────────────────────

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

// Si val est une constante pliée "$n", alloue un temp et émet ldconst.
// Sinon retourne val tel quel (c'est déjà un registre IR).
string CodeGenVisitor::materialize(const string& val) {
    if (isConst(val)) {
        string tmp = cfg->create_new_tempvar(INT32);
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {tmp, val.substr(1)});
        return tmp;
    }
    return val;
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
    
    // recurerer le type de retour preciser avant le nom de la fonction (void ou int)
    // ctx->getStart() gives us the first token in the rule
    string returnTypeText = ctx->getStart()->getText();
    Type returnType = (returnTypeText == "void") ? VOID : INT32;   

    // on suppose que les fonctions n'ont pas de paramètres pour l'instant et return soit void soit int
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


    scopeRename.push_back({}); // nouvelle table de renommage pour la fonction
     // Générer un return implicite si la fonction n'en a pas ??
    
    this->visit(ctx->block());
    
    scopeRename.pop_back();
    cfg->gen_asm(cout);
    cfg = old_cfg;
        return 0;
}

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
        if (isConst(exprVar)) {
            cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {"!ret", exprVar.substr(1)});
        }
        else {
            // Même optimisation que visitAssign
            auto& instrs = cfg->current_bb->instrs;
            if (!instrs.empty() && instrs.back()->params[0] == exprVar) {
                instrs.back()->params[0] = "!ret";
            } else {
                cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {"!ret", exprVar});
            }
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
    for (auto item : ctx->declItem()) {
        string originalName = item->ID()->getText();
        string uniqueName = originalName + "_" + to_string(cfg->getNextIndex());
        cfg->add_to_symbol_table(uniqueName, INT32);
        scopeRename.back()[originalName] = uniqueName;
        // Si le declItem a une expression d'initialisation, on la compile
        // et on copie le résultat dans la variable nouvellement déclarée
        if (item->expr()) {
            string exprVar = any_cast<string>(this->visit(item->expr()));
            if (isConst(exprVar))
                cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {uniqueName, exprVar.substr(1)});
            else
                cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {uniqueName, exprVar});
        }
    }
    return 0;
}

std::any CodeGenVisitor::visitAssign(ifccParser::AssignContext *ctx)
{
    string varName = resolve(ctx->ID()->getText());
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    if (isConst(exprVar)) {
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {varName, exprVar.substr(1)});
    }
    else {
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
    }

    return varName;
}

std::any CodeGenVisitor::visitExprConst(ifccParser::ExprConstContext *ctx)
{
    // Retourne une sentinelle "$n" : aucune IRInstr émise ici.
    // L'émission du ldconst est différée au premier point d'utilisation.
    return makeConst(stoi(ctx->CONST()->getText()));
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
    // Retourne une sentinelle "$n" : aucune IRInstr émise ici.
    return makeConst(value);
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
    if (isConst(exprVar)) return makeConst(-getConst(exprVar));  // fold : -(3) → $-3
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::unary_minus, INT32, {destVar, exprVar});
    return destVar;
}

std::any CodeGenVisitor::visitExprUnaryNot(ifccParser::ExprUnaryNotContext *ctx)
{
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    if (isConst(exprVar)) return makeConst(getConst(exprVar) == 0 ? 1 : 0);  // fold : !0 → $1
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::unary_not, INT32, {destVar, exprVar});
    return destVar;
}

// Opérations binaires
std::any CodeGenVisitor::visitExprAdd(ifccParser::ExprAddContext *ctx)
{
    string leftVar  = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string opText   = ctx->children[1]->getText();

    // Constant folding : les deux valeurs sont connues à la compilation
    if (isConst(leftVar) && isConst(rightVar)) {
        int l = getConst(leftVar), r = getConst(rightVar);
        return makeConst(opText == "+" ? l + r : l - r);
    }
    // Élimination des éléments neutres (0 pour + et -)
    if (opText == "+") {
        if (isConst(rightVar) && getConst(rightVar) == 0) return leftVar;
        if (isConst(leftVar)  && getConst(leftVar)  == 0) return rightVar;
    } else {
        if (isConst(rightVar) && getConst(rightVar) == 0) return leftVar;
    }

    string lv = materialize(leftVar);
    string rv = materialize(rightVar);
    string destVar = cfg->create_new_tempvar(INT32);
    if (opText == "+")
        cfg->current_bb->add_IRInstr(IRInstr::add, INT32, {destVar, lv, rv});
    else
        cfg->current_bb->add_IRInstr(IRInstr::sub, INT32, {destVar, lv, rv});
    return destVar;
}

std::any CodeGenVisitor::visitExprMult(ifccParser::ExprMultContext *ctx)
{
    string leftVar  = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string opText   = ctx->children[1]->getText();

    // Constant folding
    if (isConst(leftVar) && isConst(rightVar)) {
        int l = getConst(leftVar), r = getConst(rightVar);
        if (opText == "*") return makeConst(l * r);
        if (r != 0) return makeConst(opText == "/" ? l / r : l % r);
        // division par zéro : on laisse l'IR générer (comportement indéfini en C)
    }
    // Élimination des éléments neutres / absorbants
    if (opText == "*") {
        if (isConst(rightVar) && getConst(rightVar) == 1) return leftVar;   // x*1 → x
        if (isConst(leftVar)  && getConst(leftVar)  == 1) return rightVar;  // 1*x → x
        if (isConst(rightVar) && getConst(rightVar) == 0) return makeConst(0); // x*0 → 0
        if (isConst(leftVar)  && getConst(leftVar)  == 0) return makeConst(0); // 0*x → 0
    }
    if (opText == "/" && isConst(rightVar) && getConst(rightVar) == 1) return leftVar; // x/1 → x
    if (opText == "%" && isConst(rightVar) && getConst(rightVar) == 1) return makeConst(0); // x%1 → 0

    string lv = materialize(leftVar);
    string rv = materialize(rightVar);
    string destVar = cfg->create_new_tempvar(INT32);
    if (opText == "*")
        cfg->current_bb->add_IRInstr(IRInstr::mul, INT32, {destVar, lv, rv});
    else if (opText == "/")
        cfg->current_bb->add_IRInstr(IRInstr::div, INT32, {destVar, lv, rv});
    else
        cfg->current_bb->add_IRInstr(IRInstr::mod, INT32, {destVar, lv, rv});
    return destVar;
}

// Bit-à-bit
std::any CodeGenVisitor::visitExprBitOr(ifccParser::ExprBitOrContext *ctx)
{
    string leftVar  = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    if (isConst(leftVar) && isConst(rightVar))
        return makeConst(getConst(leftVar) | getConst(rightVar));
    // Élément neutre : x|0 → x
    if (isConst(rightVar) && getConst(rightVar) == 0) return leftVar;
    if (isConst(leftVar)  && getConst(leftVar)  == 0) return rightVar;
    string lv = materialize(leftVar), rv = materialize(rightVar);
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::bit_or, INT32, {destVar, lv, rv});
    return destVar;
}

std::any CodeGenVisitor::visitExprBitXor(ifccParser::ExprBitXorContext *ctx)
{
    string leftVar  = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    if (isConst(leftVar) && isConst(rightVar))
        return makeConst(getConst(leftVar) ^ getConst(rightVar));
    // Élément neutre : x^0 → x
    if (isConst(rightVar) && getConst(rightVar) == 0) return leftVar;
    if (isConst(leftVar)  && getConst(leftVar)  == 0) return rightVar;
    string lv = materialize(leftVar), rv = materialize(rightVar);
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::bit_xor, INT32, {destVar, lv, rv});
    return destVar;
}

std::any CodeGenVisitor::visitExprBitAnd(ifccParser::ExprBitAndContext *ctx)
{
    string leftVar  = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    if (isConst(leftVar) && isConst(rightVar))
        return makeConst(getConst(leftVar) & getConst(rightVar));
    // Élément absorbant : x&0 → 0
    if (isConst(rightVar) && getConst(rightVar) == 0) return makeConst(0);
    if (isConst(leftVar)  && getConst(leftVar)  == 0) return makeConst(0);
    string lv = materialize(leftVar), rv = materialize(rightVar);
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::bit_and, INT32, {destVar, lv, rv});
    return destVar;
}

// Comparaisons
std::any CodeGenVisitor::visitExprEq(ifccParser::ExprEqContext *ctx)
{
    string leftVar  = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string opText   = ctx->children[1]->getText();
    if (isConst(leftVar) && isConst(rightVar)) {
        int l = getConst(leftVar), r = getConst(rightVar);
        return makeConst(opText == "==" ? (l == r ? 1 : 0) : (l != r ? 1 : 0));
    }
    string lv = materialize(leftVar), rv = materialize(rightVar);
    string destVar = cfg->create_new_tempvar(INT32);
    if (opText == "==")
        cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, INT32, {destVar, lv, rv});
    else
        cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, INT32, {destVar, lv, rv});
    return destVar;
}

std::any CodeGenVisitor::visitExprCmp(ifccParser::ExprCmpContext *ctx)
{
    string leftVar  = any_cast<string>(this->visit(ctx->expr(0)));
    string rightVar = any_cast<string>(this->visit(ctx->expr(1)));
    string opText   = ctx->children[1]->getText();
    if (isConst(leftVar) && isConst(rightVar)) {
        int l = getConst(leftVar), r = getConst(rightVar);
        return makeConst(opText == "<" ? (l < r ? 1 : 0) : (l > r ? 1 : 0));
    }
    string lv = materialize(leftVar), rv = materialize(rightVar);
    string destVar = cfg->create_new_tempvar(INT32);
    if (opText == "<")
        cfg->current_bb->add_IRInstr(IRInstr::cmp_lt, INT32, {destVar, lv, rv});
    else
        cfg->current_bb->add_IRInstr(IRInstr::cmp_gt, INT32, {destVar, lv, rv});
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
        argVar = materialize(argVar);
        argVars.push_back(argVar);
    }
    
    // Construire les paramètres de l'instruction IR call
    // Format: {nom_fonction, var_retour, arg1, arg2, ...}
    vector<string> callParams = {functionName, ""};  // "" = pas de retour utilisé
    callParams.insert(callParams.end(), argVars.begin(), argVars.end());
    
    cfg->current_bb->add_IRInstr(IRInstr::call, INT32, callParams);
    
    return 0;
}

// Appel de fonction en tant qu'expression (ex: x = getchar();)
std::any CodeGenVisitor::visitExprCall(ifccParser::ExprCallContext *ctx)
{
    string functionName = ctx->ID()->getText();
    
    // Évaluer l'argument s'il existe
    vector<string> argVars;
    if (ctx->expr()) {
        string argVar = any_cast<string>(visit(ctx->expr()));
        argVar = materialize(argVar);
        argVars.push_back(argVar);
    }
    
    // Créer une variable temporaire pour le retour
    string retVar = cfg->create_new_tempvar(INT32);
    
    // Construire les paramètres de l'instruction IR call
    vector<string> callParams = {functionName, retVar};
    callParams.insert(callParams.end(), argVars.begin(), argVars.end());
    
    cfg->current_bb->add_IRInstr(IRInstr::call, INT32, callParams);
    
    return retVar;  // Retourne la variable contenant le résultat
}

std::any CodeGenVisitor::visitExprFonctCall(ifccParser::ExprFonctCallContext *ctx)
{
    // appel fonction sans paramètres pour l'instant
    string fctName = ctx->ID()->getText();
    // Crée une variable temporaire pour stocker le résultat
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::call, INT32, {fctName, destVar});
    
    return destVar;
}


