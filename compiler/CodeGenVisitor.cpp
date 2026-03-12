#include "CodeGenVisitor.h"
#include <unordered_map>
#include <string>

CodeGenVisitor::CodeGenVisitor(DefFonction* ast) {
    cfg = new CFG(ast);
    BasicBlock* bb = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(bb);
    cfg->add_to_symbol_table("!ret", INT32);
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
            cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, exprVar});
        }
    }
    return 0;
}

std::any CodeGenVisitor::visitAssign(ifccParser::AssignContext *ctx)
{
    string varName = ctx->ID()->getText();
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, exprVar});
    return varName;
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
    string varName = ctx->ID()->getText();
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


