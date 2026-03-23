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
    cfg->add_to_symbol_table("!ret", INT32);
}

std::any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    this->visit(ctx->block());
    cfg->gen_asm(cout);
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
    string exprVar = any_cast<string>(this->visit(ctx->expr()));
    cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {"!ret", exprVar});
    
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

std::any CodeGenVisitor::visitAssign(ifccParser::AssignContext *ctx)
{
    string varName = resolve(ctx->ID()->getText());
    string exprVar = any_cast<string>(this->visit(ctx->expr()));

    string op = ctx->children[1]->getText();

    if (op == "=") {
        cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, exprVar});
    }
    else {
        string tmp = cfg->create_new_tempvar(INT32);

        if (op == "+=")
            cfg->current_bb->add_IRInstr(IRInstr::add, INT32, {tmp, varName, exprVar});
        else if (op == "-=")
            cfg->current_bb->add_IRInstr(IRInstr::sub, INT32, {tmp, varName, exprVar});
        else if (op == "*=")
            cfg->current_bb->add_IRInstr(IRInstr::mul, INT32, {tmp, varName, exprVar});
        else if (op == "/=")
            cfg->current_bb->add_IRInstr(IRInstr::div, INT32, {tmp, varName, exprVar});
        else if (op == "%=")
            cfg->current_bb->add_IRInstr(IRInstr::mod, INT32, {tmp, varName, exprVar});

        // store result back
        cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, tmp});
    }

    return varName;
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


std::any CodeGenVisitor::visitIfStmt(ifccParser::IfStmtContext *ctx) { 
    // 1 évaluer la condition
    string condVar = any_cast<string>(visit(ctx->expr()));

    // 2️ créer les blocs
    BasicBlock* bb_if   = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock* bb_else = nullptr;
    BasicBlock* bb_end  = new BasicBlock(cfg, cfg->new_BB_name());

    if (ctx->stmt().size() == 2) {
        bb_else = new BasicBlock(cfg, cfg->new_BB_name());
    }

    // 3 configurer le bloc courant
    cfg->current_bb->test_var_name = condVar;
    cfg->current_bb->exit_true = bb_if;

    if (bb_else)
        cfg->current_bb->exit_false = bb_else;
    else
        cfg->current_bb->exit_false = bb_end;

    // 4 bloc IF
    cfg->add_bb(bb_if);
    visit(ctx->stmt(0));

    cfg->current_bb->exit_true = bb_end;
    cfg->current_bb->exit_false = nullptr;

    // 5 bloc ELSE
    if (bb_else) {
        cfg->add_bb(bb_else);
        visit(ctx->stmt(1));

        cfg->current_bb->exit_true = bb_end;
        cfg->current_bb->exit_false = nullptr;
    }

    // 6 bloc END
    cfg->add_bb(bb_end);

    return 0;
}

std::any CodeGenVisitor::visitWhileStmt(ifccParser::WhileStmtContext *ctx) {

    // condition block
    BasicBlock* bb_cond = new BasicBlock(cfg, cfg->new_BB_name());

    // body block
    BasicBlock* bb_body = new BasicBlock(cfg, cfg->new_BB_name());

    // end block
    BasicBlock* bb_end = new BasicBlock(cfg, cfg->new_BB_name());

    // jump from current block to condition
    cfg->current_bb->exit_true = bb_cond;
    cfg->current_bb->exit_false = nullptr;

    // CONDITION
    cfg->add_bb(bb_cond);
    string condVar = any_cast<string>(visit(ctx->expr()));

    cfg->current_bb->test_var_name = condVar;
    cfg->current_bb->exit_true = bb_body;
    cfg->current_bb->exit_false = bb_end;

    // BODY
    cfg->add_bb(bb_body);
    visit(ctx->stmt());

    // after body go back to condition
    cfg->current_bb->exit_true = bb_cond;
    cfg->current_bb->exit_false = nullptr;

    // END
    cfg->add_bb(bb_end);

    return 0;
}

std::any CodeGenVisitor::visitIncdec(ifccParser::IncdecContext *ctx)
{
    string varName = resolve(ctx->ID()->getText());

    string one = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {one, "1"});

    string tmp = cfg->create_new_tempvar(INT32);

    string op = ctx->op->getText();

    if (op == "++") {
        cfg->current_bb->add_IRInstr(IRInstr::add, INT32, {tmp, varName, one});
    } else {
        cfg->current_bb->add_IRInstr(IRInstr::sub, INT32, {tmp, varName, one});
    }

    cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {varName, tmp});

    return 0;
}

std::any CodeGenVisitor::visitExprAnd(ifccParser::ExprAndContext *ctx)
{
    string leftVar = any_cast<string>(visit(ctx->expr(0)));
    string destVar = cfg->create_new_tempvar(INT32);

    BasicBlock* bb_right = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock* bb_false = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock* bb_end   = new BasicBlock(cfg, cfg->new_BB_name());

    cfg->current_bb->test_var_name = leftVar;
    cfg->current_bb->exit_true = bb_right;
    cfg->current_bb->exit_false = bb_false;

    // FALSE → 0
    cfg->add_bb(bb_false);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {destVar, "0"});
    cfg->current_bb->exit_true = bb_end;

    // RIGHT
    cfg->add_bb(bb_right);
    string rightVar = any_cast<string>(visit(ctx->expr(1)));

    string zero = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {zero, "0"});

    cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, INT32, {destVar, rightVar, zero});
    cfg->current_bb->exit_true = bb_end;

    cfg->add_bb(bb_end);

    return destVar;
}

std::any CodeGenVisitor::visitExprOr(ifccParser::ExprOrContext *ctx)
{
    string leftVar = any_cast<string>(visit(ctx->expr(0)));
    string destVar = cfg->create_new_tempvar(INT32);

    BasicBlock* bb_true  = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock* bb_right = new BasicBlock(cfg, cfg->new_BB_name());
    BasicBlock* bb_end   = new BasicBlock(cfg, cfg->new_BB_name());

    // TEST left
    cfg->current_bb->test_var_name = leftVar;
    cfg->current_bb->exit_true = bb_true;    // si left != 0 → TRUE
    cfg->current_bb->exit_false = bb_right;  // sinon → évaluer right

    // TRUE → résultat = 1
    cfg->add_bb(bb_true);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {destVar, "1"});
    cfg->current_bb->exit_true = bb_end;

    // RIGHT → évaluer et convertir en bool
    cfg->add_bb(bb_right);
    string rightVar = any_cast<string>(visit(ctx->expr(1)));

    string zero = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {zero, "0"});

    // destVar = (rightVar != 0)
    cfg->current_bb->add_IRInstr(IRInstr::cmp_ne, INT32, {destVar, rightVar, zero});
    cfg->current_bb->exit_true = bb_end;

    // END
    cfg->add_bb(bb_end);

    return destVar;
}