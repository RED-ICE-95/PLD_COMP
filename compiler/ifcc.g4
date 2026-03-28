grammar ifcc;

axiom : prog EOF ;

prog : fonctDecl* 'int' 'main' '(' ')' block ;

fonctDecl : ('void' | 'int') ID '(' list_decl_param ')' block ;

stmt
    : (return_stmt | assign | declar | call_stmt | incdec) ';'
    | if_stmt
    | while_stmt
    | block
    ;

incdec : ID op=('++' | '--') ;

return_stmt : RETURN expr ;

assign
    : ID '=' expr                  # assignSimple
    | ID '[' expr ']' '=' expr     # assignArray
    | ID '+=' expr                 # assignAdd
    | ID '-=' expr                 # assignSub
    | ID '*=' expr                 # assignMul
    | ID '/=' expr                 # assignDiv
    | ID '%=' expr                 # assignMod
    ;

declar : 'int' declItem (',' declItem)* ;

declItem
    : ID ('=' expr)?
    | ID '[' CONST ']' ('=' '{' exprList? '}')?
    ;

exprList : expr (',' expr)* ;

block : '{' stmt* '}' ;

call_stmt : ID '(' expr? ')' ;

list_decl_param : ('int' ID (',' 'int' ID)*)? ;
list_param : (expr (',' expr)*)? ;

if_stmt    : 'if' '(' expr ')' stmt ('else' stmt)? ;
while_stmt : 'while' '(' expr ')' stmt ;

expr
    : '-' expr                             # exprUnaryMinus
    | '!' expr                             # exprUnaryNot
    | '(' expr ')'                         # exprParen
    | CONST                                # exprConst
    | CHAR_CONST                           # exprCharConst
    | ID '(' list_param ')'               # exprFonctCall
    | ID '[' expr ']'                      # exprArrayAccess
    | ID                                   # exprId
    | expr ('*' | '/' | '%') expr          # exprMult
    | expr ('+' | '-') expr                # exprAdd
    | expr ('<' | '>') expr                # exprCmp
    | expr ('==' | '!=') expr              # exprEq
    | expr '&' expr                        # exprBitAnd
    | expr '^' expr                        # exprBitXor
    | expr '|' expr                        # exprBitOr
    | expr '&&' expr                       # exprAnd
    | expr '||' expr                       # exprOr
    ;

RETURN : 'return' ;
WHILE  : 'while' ;
IF     : 'if' ;
ELSE   : 'else' ;
CONST : [0-9]+ ;
CHAR_CONST : '\'' (~['\\] | '\\' .) '\'' ;
ID : [a-zA-Z_][a-zA-Z0-9_]* ;
COMMENT : '/*' .*? '*/' -> skip ;
LINE_COMMENT : '//' ~[\r\n]* -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS : [ \t\r\n] -> channel(HIDDEN) ;