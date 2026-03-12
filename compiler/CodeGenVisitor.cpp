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
    BasicBlock* bb = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(bb);
    cfg->add_to_symbol_table("!ret", INT32);
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
    for (auto sctx : ctx->stmt()) {
        this->visit(sctx);
    }
    cfg->gen_asm(cout);
    return 0;
}

std::any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    // Constante pliée → ldconst direct vers !ret (pas de copy intermédiaire)
    if (isConst(exprVar))
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {"!ret", exprVar.substr(1)});
    else
        cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {"!ret", exprVar});
    return 0;
}

std::any CodeGenVisitor::visitDeclar(ifccParser::DeclarContext *ctx)
{
    for (auto item : ctx->declItem()) {
        string varName = item->ID()->getText();
        cfg->add_to_symbol_table(varName, INT32);
        // Si le declItem a une expression d'initialisation, on la compile
        // et on copie le résultat dans la variable nouvellement déclarée
        if (item->expr()) {
            string exprVar = any_cast<string>(this->visit(item->expr()));
            if (isConst(exprVar))
                cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {varName, exprVar.substr(1)});
            else
                cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, exprVar});
        }
    }
    return 0;
}

std::any CodeGenVisitor::visitAssign(ifccParser::AssignContext *ctx)
{
    string varName = ctx->ID()->getText();
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    if (isConst(exprVar))
        cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {varName, exprVar.substr(1)});
    else
        cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, exprVar});
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
    // On retourne directement le nom de la variable : pas besoin d'un copy
    // vers un temporaire, l'IR travaille avec des noms symboliques.
    return ctx->ID()->getText();
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


