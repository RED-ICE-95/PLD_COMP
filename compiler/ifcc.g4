grammar ifcc;

axiom : prog EOF ;

prog : fonctDecl* 'int' 'main' '(' ')' '{' stmt* '}' ;

fonctDecl : ('void'|'int') ID '(' ')' '{' stmt* '}';
stmt :(return_stmt | assign | declar) ';' ;
return_stmt : RETURN expr ;
assign : ID '=' expr ;
declar : 'int' ID (',' ID)* ;

expr : '-' expr                            # exprUnaryMinus
    | '!' expr                            # exprUnaryNot
    | '(' expr ')'                        # exprParen
    | CONST                               # exprConst
    | CHAR_CONST                          # exprCharConst
    | ID '(' ')'                          # exprFonctCall
    | ID                                  # exprId
    | expr ('*' | '/' | '%') expr         # exprMult
    | expr ('+' | '-') expr               # exprAdd
    | expr ('<' | '>') expr               # exprCmp
    | expr ('==' | '!=') expr             # exprEq
    | expr '&' expr                       # exprBitAnd
    | expr '^' expr                       # exprBitXor
    | expr '|' expr                       # exprBitOr
    ;

RETURN : 'return' ;
CONST : [0-9]+ ;
CHAR_CONST : '\'' (~['\\] | '\\' .) '\'' ;
ID : [a-zA-Z_][a-zA-Z0-9_]* ;
COMMENT : '/*' .*? '*/' -> skip ;
LINE_COMMENT : '//' ~[\r\n]* -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
