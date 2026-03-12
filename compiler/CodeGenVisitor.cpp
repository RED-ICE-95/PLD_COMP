#include "CodeGenVisitor.h"
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>

// pile de scopes pour gérer shadowing
std::vector<std::unordered_map<std::string,int>> scopeOffsets;
int currentOffset = 0;

// constructeur
CodeGenVisitor::CodeGenVisitor() {
    currentOffset = 0;
}

// allouer un offset temporaire pour opérations intermédiaires
int CodeGenVisitor::allocTemp() {
    currentOffset -= 4;
    std::cout << "    sub $4, %rsp\n";
    return currentOffset;
}

// lookup variable depuis la pile de scopes
int CodeGenVisitor::lookupVar(const std::string &name) {
    for (int i = scopeOffsets.size()-1; i >= 0; i--) {
        auto it = scopeOffsets[i].find(name);
        if (it != scopeOffsets[i].end())
            return it->second;
    }
    std::cerr << "Erreur codegen : variable " << name << " non trouvée\n";
    return 0;
}

// =========================== VISITORS ===========================

// programme principal
antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    #ifdef __APPLE__
    std::cout<< ".globl _main\n";
    std::cout<< " _main:\n";
    #else
    std::cout<< ".globl main\n";
    std::cout<< " main:\n";
    #endif

    std::cout << "    push %rbp\n";
    std::cout << "    mov %rsp, %rbp\n";

    // premier scope global
    scopeOffsets.push_back({});
    currentOffset = 0;

    for (auto sctx : ctx->stmt()) {
        this->visit(sctx);
    }

    scopeOffsets.pop_back();

    std::cout << "    mov %rbp, %rsp\n";
    std::cout << "    pop %rbp\n";
    std::cout << "    ret\n";

    return 0;
}

// blocs {}
antlrcpp::Any CodeGenVisitor::visitBlock(ifccParser::BlockContext *ctx) {
    // nouveau scope
    scopeOffsets.push_back({});
    int oldOffset = currentOffset;

    visitChildren(ctx); // instructions du bloc

    currentOffset = oldOffset; // restaurer offset précédent
    scopeOffsets.pop_back();
    return nullptr;
}

// déclarations
antlrcpp::Any CodeGenVisitor::visitDeclar(ifccParser::DeclarContext *ctx) {
    auto &currentScope = scopeOffsets.back();
    for (auto id : ctx->ID()) {
        std::string name = id->getText();
        currentOffset -= 4;
        currentScope[name] = currentOffset;
        std::cout << "    sub $4, %rsp\n";
    }
    return nullptr;
}

// assignation
antlrcpp::Any CodeGenVisitor::visitAssign(ifccParser::AssignContext *ctx) {
    this->visit(ctx->expr()); // valeur dans %eax
    int offset = lookupVar(ctx->ID()->getText());
    std::cout << "    movl %eax, " << offset << "(%rbp)\n";
    return nullptr;
}
//return
antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    this->visit(ctx->expr()); // résultat dans %eax

    std::cout << "    mov %rbp, %rsp\n";
    std::cout << "    pop %rbp\n";
    std::cout << "    ret\n";

    return nullptr;
}
// EXPR 

// constantes
antlrcpp::Any CodeGenVisitor::visitExprConst(ifccParser::ExprConstContext *ctx) {
    std::cout << "    movl $" << ctx->CONST()->getText() << ", %eax\n";
    return nullptr;
}

// char constants
antlrcpp::Any CodeGenVisitor::visitExprCharConst(ifccParser::ExprCharConstContext *ctx) {
    std::string text = ctx->CHAR_CONST()->getText(); // ex: 'Z' ou '\n'
    std::string inner = text.substr(1, text.size() - 2);
    int value = 0;
    if (inner.size() == 1) {
        value = (int)(unsigned char)inner[0];
    } else if (inner.size() == 2 && inner[0] == '\\') {
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
    std::cout << "    movl $" << value << ", %eax\n";
    return nullptr;
}

// accès variable
antlrcpp::Any CodeGenVisitor::visitExprId(ifccParser::ExprIdContext *ctx) {
    int offset = lookupVar(ctx->ID()->getText());
    std::cout << "    movl " << offset << "(%rbp), %eax\n";
    return nullptr;
}

// opérateurs unaires
antlrcpp::Any CodeGenVisitor::visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *ctx) {
    this->visit(ctx->expr());
    std::cout << "    negl %eax\n";
    return nullptr;
}

antlrcpp::Any CodeGenVisitor::visitExprUnaryNot(ifccParser::ExprUnaryNotContext *ctx) {
    this->visit(ctx->expr());
    std::cout << "    cmpl $0, %eax\n";
    std::cout << "    sete %al\n";
    std::cout << "    movzbl %al, %eax\n";
    return nullptr;
}

antlrcpp::Any CodeGenVisitor::visitExprParen(ifccParser::ExprParenContext *ctx) {
    this->visit(ctx->expr());
    return nullptr;
}

// opérations binaires (Add, Mult, BitAnd/Or/Xor, Cmp, Eq)
#define BINARY_OP_VISIT(NAME, ASM_OP) \
antlrcpp::Any CodeGenVisitor::visitExpr##NAME(ifccParser::Expr##NAME##Context *ctx) { \
    this->visit(ctx->expr(0)); \
    int tmp = allocTemp(); \
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n"; \
    this->visit(ctx->expr(1)); \
    std::cout << "    " << ASM_OP << " " << tmp << "(%rbp), %eax\n"; \
    std::cout << "    add $4, %rsp\n"; \
    currentOffset += 4; \
    return nullptr; \
}

BINARY_OP_VISIT(Add, "addl")
BINARY_OP_VISIT(Mult, "imull")
BINARY_OP_VISIT(BitOr, "orl")
BINARY_OP_VISIT(BitXor, "xorl")
BINARY_OP_VISIT(BitAnd, "andl")

// Comparaisons
antlrcpp::Any CodeGenVisitor::visitExprCmp(ifccParser::ExprCmpContext *ctx) {
    this->visit(ctx->expr(0));
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";
    this->visit(ctx->expr(1));
    std::cout << "    movl " << tmp << "(%rbp), %ecx\n";
    std::cout << "    cmpl %eax, %ecx\n";
    std::string opText = ctx->children[1]->getText();
    if (opText == "<") std::cout << "    setl %al\n";
    else std::cout << "    setg %al\n";
    std::cout << "    movzbl %al, %eax\n";
    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return nullptr;
}

antlrcpp::Any CodeGenVisitor::visitExprEq(ifccParser::ExprEqContext *ctx) {
    this->visit(ctx->expr(0));
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";
    this->visit(ctx->expr(1));
    std::cout << "    movl " << tmp << "(%rbp), %ecx\n";
    std::cout << "    cmpl %eax, %ecx\n";
    std::string opText = ctx->children[1]->getText();
    if (opText == "==") std::cout << "    sete %al\n";
    else std::cout << "    setne %al\n";
    std::cout << "    movzbl %al, %eax\n";
    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return nullptr;
}