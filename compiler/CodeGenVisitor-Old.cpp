antlrcpp::Any CodeGenVisitor::visitExprConst(ifccParser::ExprConstContext *ctx)
{
    std::cout << "    movl $" << ctx->CONST()->getText() << ", %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprCharConst(ifccParser::ExprCharConstContext *ctx)
{
    std::string text = ctx->CHAR_CONST()->getText(); 
    std::string inner = text.substr(1, text.size() - 2);    // on enlève guillemets
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
    std::cout << "    movl $" << value << ", %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprId(ifccParser::ExprIdContext *ctx)
{
    std::string varName = ctx->ID()->getText();
    std::cout << "    movl " << varOffsets[varName] << "(%rbp), %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprUnaryMinus(ifccParser::ExprUnaryMinusContext *ctx)
{
    this->visit(ctx->expr());
    std::cout << "    negl %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprAdd(ifccParser::ExprAddContext *ctx)
{
    this->visit(ctx->expr(0));              // gauche dans %eax
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";

    this->visit(ctx->expr(1));              // droite dans %eax

    std::string opText = ctx->children[1]->getText();
    if (opText == "+") {
        std::cout << "    addl " << tmp << "(%rbp), %eax\n";
    } else {
        std::cout << "    movl " << tmp << "(%rbp), %ecx\n";
        std::cout << "    subl %eax, %ecx\n";
        std::cout << "    movl %ecx, %eax\n";
    }

    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitExprMult(ifccParser::ExprMultContext *ctx)
{
    this->visit(ctx->expr(0));              // gauche dans %eax
    int tmp = allocTemp();
    std::cout << "    movl %eax, " << tmp << "(%rbp)\n";

    this->visit(ctx->expr(1));              // droite dans %eax

    std::string opText = ctx->children[1]->getText();
    if (opText == "*") {
        std::cout << "    imull " << tmp << "(%rbp), %eax\n";
    } else if (opText == "/") {
        std::cout << "    movl %eax, %ecx\n";
        std::cout << "    movl " << tmp << "(%rbp), %eax\n";
        std::cout << "    cdq\n";
        std::cout << "    idivl %ecx\n";
    } else { // %
        std::cout << "    movl %eax, %ecx\n";
        std::cout << "    movl " << tmp << "(%rbp), %eax\n";
        std::cout << "    cdq\n";
        std::cout << "    idivl %ecx\n";
        std::cout << "    movl %edx, %eax\n";
    }

    std::cout << "    add $4, %rsp\n";
    currentOffset += 4;
    return 0;
}


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