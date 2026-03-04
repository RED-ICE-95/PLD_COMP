grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* '}' ;

stmt :(return_stmt | assign | declar) ';' ;
return_stmt : RETURN expr ;
assign : ID '=' expr ;
declar : 'int' ID (',' ID)* ;

expr : expr '|' expr                        # exprBitOr
    | expr '^' expr                         # exprBitXor
    | expr '&' expr                         # exprBitAnd
    | expr ('==' | '!=') expr              # exprEq
    | expr ('<' | '>') expr               # exprCmp
    | expr ('+' | '-') expr               # exprAdd
    | expr ('*' | '/' | '%') expr         # exprMult
    | '-' expr                            # exprUnaryMinus
    | '!' expr                            # exprUnaryNot
    | '(' expr ')'                        # exprParen
    | CONST                               # exprConst
    | ID                                  # exprId
    ;

RETURN : 'return' ;
CONST : [0-9]+ ;
ID : [a-zA-Z_][a-zA-Z0-9_]* ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
