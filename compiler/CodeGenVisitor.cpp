#include "CodeGenVisitor.h"
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> varOffsets;
int currentOffset = 0;

// constructeur : on récupèrera les offsets de variables depuis SymbolTableVisitor
CodeGenVisitor::CodeGenVisitor(std::unordered_map<std::string, int> offsets)
    : varOffsets(std::move(offsets)), currentOffset(0){
    // on cherche l'offset le plus bas (ex: -12 pour 3 variables)
    int minOffset = 0;
    for (auto& [name, offset] : varOffsets) {
        minOffset = std::min(minOffset, offset);
    }

    // on inverse les offsets : ex: -4 → -12, -8 → -8, -12 → -4
    for (auto& [name, offset] : varOffsets) {
        offset = minOffset - offset - 4;
    }
    // currentOffset = min des offsets remappés (= adresse la plus basse utilisée)
    currentOffset = 0;
    for (auto& [name, offset] : varOffsets) {
        currentOffset = std::min(currentOffset, offset);
    }
}


// Pour les opérations binaires, il faudra stocker résultat intermédiaire
int CodeGenVisitor::allocTemp() {
    currentOffset -= 4;
    std::cout << "    sub $4, %rsp\n";
    return currentOffset;
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    #ifdef __APPLE__
    std::cout<< ".globl _main\n" ;
    std::cout<< " _main: \n" ;
    #else
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;
    #endif

    std::cout << "    push %rbp\n";
    std::cout << "    mov %rsp, %rbp\n";

    for (auto sctx : ctx->stmt()) {
        this->visit(sctx);
    }

    std::cout << "    mov %rbp, %rsp\n";
    std::cout << "    pop %rbp\n";

    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    // on délègue à visitExpr et on obtient la valeur dans %eax (CONST ou ID)
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclar(ifccParser::DeclarContext *ctx) {
    for (auto id : ctx->ID()) {
        std::cout << "    sub $4, %rsp\n";
    }
    return 0;
}


antlrcpp::Any CodeGenVisitor::visitAssign(ifccParser::AssignContext *ctx) {
    std::string varName = ctx->ID()->getText();
    // on délègue à visitExpr, puis on récupère la valeur dans %eax 
    this->visit(ctx->expr());
    std::cout << "    movl %eax, " << varOffsets[varName] << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprConst(ifccParser::ExprConstContext *ctx)
{
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {destVar, ctx->CONST()->getText()});
    return destVar;
}

antlrcpp::Any CodeGenVisitor::visitExprCharConst(ifccParser::ExprCharConstContext *ctx)
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

antlrcpp::Any CodeGenVisitor::visitExprId(ifccParser::ExprIdContext *ctx)
{
    string varName = ctx->ID()->getText();
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::copy, INT32, {destVar, varName});
    return destVar;
}

antlrcpp::Any CodeGenVisitor::visitExprParen(ifccParser::ExprParenContext *ctx)
{
    return this->visit(ctx->expr());
}

// Opérations unaires
antlrcpp::Any CodeGenVisitor::visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *ctx)
{
    // on stocke zero et on fait soustraction : 0 - expr
    string exprVar = this->visit(ctx->expr()).as<string>();
    string zero = cfg->create_new_tempvar(INT32);
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::sub, INT32, {destVar, zero, exprVar});
    return destVar;
}

antlrcpp::Any CodeGenVisitor::visitExprUnaryNot(ifccParser::ExprUnaryNotContext *ctx)
{
    // on stocke zero et on fait comparaison : 0 == expr
    string exprVar = this->visit(ctx->expr()).as<string>();
    string zero = cfg->create_new_tempvar(INT32);
    string destVar = cfg->create_new_tempvar(INT32);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, INT32, {zero, "0"});
    cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, INT32, {destVar, zero, exprVar});
    return destVar;
}

// Opérations binaires
antlrcpp::Any CodeGenVisitor::visitExprAdd(ifccParser::ExprAddContext *ctx)
{
    string leftVar = this->visit(ctx->expr(0)).as<string>();
    string rightVar = this->visit(ctx->expr(1)).as<string>();
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

antlrcpp::Any CodeGenVisitor::visitExprMult(ifccParser::ExprMultContext *ctx)
{
    string leftVar = this->visit(ctx->expr(0)).as<string>();
    string rightVar = this->visit(ctx->expr(1)).as<string>();
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
antlrcpp::Any CodeGenVisitor::visitExprBitOr(ifccParser::ExprBitOrContext *ctx)
{
    this->visit(ctx->expr(0));
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";
    this->visit(ctx->expr(1));
    std::cout << "    orl " << tmp << "(%rbp), %eax\n";
    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprBitXor(ifccParser::ExprBitXorContext *ctx)
{
    this->visit(ctx->expr(0));
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";
    this->visit(ctx->expr(1));
    std::cout << "    xorl " << tmp << "(%rbp), %eax\n";
    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprBitAnd(ifccParser::ExprBitAndContext *ctx)
{
    this->visit(ctx->expr(0));
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";
    this->visit(ctx->expr(1));
    std::cout << "    andl " << tmp << "(%rbp), %eax\n";
    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return 0;
}

// Comparaisons
antlrcpp::Any CodeGenVisitor::visitExprEq(ifccParser::ExprEqContext *ctx)
{
    this->visit(ctx->expr(0));              // gauche → %eax
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";
    this->visit(ctx->expr(1));              // droite → %eax
    std::cout << "    movl " << tmp << "(%rbp), %ecx\n";
    std::cout << "    cmpl %eax, %ecx\n";  // ecx - eax  (gauche - droite)

    std::string opText = ctx->children[1]->getText();
    if (opText == "==") {
        std::cout << "    sete %al\n";
    } else { // !=
        std::cout << "    setne %al\n";
    }
    std::cout << "    movzbl %al, %eax\n";
    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprCmp(ifccParser::ExprCmpContext *ctx)
{
    this->visit(ctx->expr(0));              // gauche → %eax
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";
    this->visit(ctx->expr(1));              // droite → %eax
    std::cout << "    movl " << tmp << "(%rbp), %ecx\n";
    std::cout << "    cmpl %eax, %ecx\n";  // ecx - eax  (gauche - droite)

    std::string opText = ctx->children[1]->getText();
    if (opText == "<") {
        std::cout << "    setl %al\n";
    } else { // >
        std::cout << "    setg %al\n";
    }
    std::cout << "    movzbl %al, %eax\n";
    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return 0;
}