%{


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#include "types.h"
#include "tree_basic.h"
#include "str.h"
#include "dbug.h"
#include "ctinfo.h"
#include "free.h"
#include "globals.h"

static node *parseresult = NULL;
extern int yylex();
static int yyerror( char *errname);

// monop toegevoegd als test.

// enable debugging
#define YYDEBUG 1

%}

%union {
 nodetype            nodetype;
 char               *id;
 int                 cint;
 float               cflt;
 binop               cbinop;
 monop               cmonop;
 type                ctype;
 node               *node;
}

%token PARENTHESIS_L PARENTHESIS_R CURLY_L CURLY_R BRACKET_L BRACKET_R COMMA SEMICOLON
%token MINUS PLUS STAR SLASH PERCENT LE LT GE GT EQ NE OR AND LET NEG NOT
%token INT FLOAT BOOL VOID TRUEVAL FALSEVAL
%token EXTERN EXPORT RETURN
%token IF ELSE DO WHILE FOR

%token <cint> NUM
%token <cflt> FLOATNUM
%token <id> ID

%type <node> intval floatval boolval constant expr
%type <node> stmts stmt assign varlet program 
%type <node> return exprstmt binop exprs monop 
%type <node> vardecl
// %type <node> fundef funbody fundefs decl decls globdecl globdef vardecls

// %type <node> fundec

%type <ctype> type

%start program

%left OR
%left AND
%left EQ NEQ
%left LT GT LET LE GE
%left PLUS MINUS
%left STAR SLASH PERCENT
%left NOT NEG

%nonassoc ID
%nonassoc PARENTHESIS_L PARENTHESIS_R

%%

program: 
    // decls
    //     {
    //         parseresult = $1;
    //     }
    // |  
    stmts 
        {
            parseresult = $1;
        }
    ;

stmts: stmt stmts
        {
            $$ = TBmakeStmts( $1, $2);
        }
    |   stmt
        {
            $$ = TBmakeStmts( $1, NULL);
        }
    ;

// decls: decl decls
//         {
//             $$ = TBmakeDecls( $1, $2);
//         }
//     |   decl
//         {
//             $$ = TBmakeDecls( $1, NULL);
//         }
//     ;

// decl: globdecl
//         {
//             $$ = $1;
//         }
//     |   globdef
//         {
//             $$ = $1;
//         }
//     |   fundefs
//         {
//             $$ = $1;
//         }
//     ;

stmt: assign
        {
            $$ = $1;
        }
    |   return
        {
            $$ = $1;
        }
    |   exprstmt
        {
            $$ = $1;
        }
    |   vardecl
        {
            $$ = $1;
        } 
    ;

// fundec: EXTERN type ID PARENTHESIS_L exprs PARENTHESIS_R SEMICOLON
//         {
//             $$ = TBmakeFundec($2, $3, $8, $4);
//         }
//     ;

// fundefs: fundef fundefs
//         {
//             $$ = TBmakeFundefs( $1, $2);
//         }
//     |   fundef
//         {
//             $$ = TBmakeFundefs( $1, NULL);
//         }
//     ;

// fundef: type ID PARENTHESIS_L exprs PARENTHESIS_R funbody
//         {
//             $$ = TBmakeFundef( $1, $2, $6, $4);
//         }
//     |   EXPORT type ID PARENTHESIS_L exprs PARENTHESIS_R funbody
//         {
//             $$ = TBmakeFundef( $2, $3, $7, $5);
//         }
//     ;

// funbody: CURLY_L vardecls fundefs stmts CURLY_R
//         {
//             $$ = TBmakeFunbody( $2, $3, $4);
//         }
//     ;

return: RETURN SEMICOLON
        {
          $$ = TBmakeReturn( NULL);
        }
    |   RETURN expr SEMICOLON
        {
          $$ = TBmakeReturn( $2);
        }
    ;

assign: varlet LET expr
        {
          $$ = TBmakeAssign( $1, $3);
        }
    ;

varlet: ID
        {
          $$ = TBmakeVarlet( STRcpy( $1), NULL, NULL);
        }
    ;

// @todo check how to pass export flag
// @todo check how to declare global def without 
// globdef: type ID SEMICOLON
//         {
//             $$ = TBmakeGlobdef($1, $2, NULL, NULL);
//         }
//     |   type ID LET expr SEMICOLON
//         {
//             $$ = TBmakeGlobdef($1, $2, NULL, $4);
//         } 
//     |   EXPORT type ID SEMICOLON
//         {
//             $$ = TBmakeGlobdef($2, $3, NULL, NULL);
//         }
//     |   EXPORT type ID LET expr SEMICOLON
//         {
//             $$ = TBmakeGlobdef($2, $3, NULL, $5);
//         }
//     ;

// globdecl: EXTERN type ID SEMICOLON
//         {
//             $$ = TBmakeGlobdecl($2, $3, NULL);
//         }
//     ;


// vardecls: vardecl vardecls
//         {
//             $$ = TBmakeVardel($1, $2);
//         }
//     |   vardecl
//         {
//             $$ = TBmakeVardels($1, NULL);
//         }
//     ;

vardecl: type ID SEMICOLON
        {
            $$ = TBmakeVardecl($2, $1, NULL, NULL, NULL);
        }
    |   type ID LET expr SEMICOLON
        {
            $$ = TBmakeVardecl($2, $1, NULL, $4, NULL);
        }
    ;

exprs: expr COMMA exprs
        {
            $$ = TBmakeExprs($1, $3);
        }
    |   expr
        {
            $$ = TBmakeExprs($1, NULL);
        }
    ;

expr: 
    constant
        {
            $$ = $1;
        }
    |   ID
        {
            $$ = TBmakeVar( STRcpy( $1), NULL, NULL);
        }
    |   binop
        {
            $$ = $1;
        }
    |   monop
        {
            $$ = $1;
        }
    |   PARENTHESIS_L type PARENTHESIS_R expr
        {
            $$ = TBmakeCast( $2, $4);
        }
    |   ID PARENTHESIS_L exprs PARENTHESIS_R
        {
          $$ = TBmakeFuncall( STRcpy( $1), NULL, $3);
        }
    |   ID PARENTHESIS_L PARENTHESIS_R
        {
            $$ = TBmakeFuncall( STRcpy( $1), NULL, NULL);
        }
    ;

exprstmt: expr SEMICOLON
        {
            $$ = TBmakeExprstmt( $1);
        }
    ;

constant: floatval
        {
            $$ = $1;
        }
    |   intval
        {
            $$ = $1;
        }
    |   boolval
        {
            $$ = $1;
        }
    ;

floatval: FLOATNUM
        {
            $$ = TBmakeFloat( $1);
        }
    ;

intval: NUM
        {
            $$ = TBmakeNum( $1);
        }
    ;

boolval: TRUEVAL
        {
            $$ = TBmakeBool( TRUE);
        }
    |   FALSEVAL
        {
            $$ = TBmakeBool( FALSE);
        }
    ;

binop: expr LE expr
        { 
            $$ = TBmakeBinop( BO_le, $1, $3);
        }
    |   expr LT expr
        { 
            $$ = TBmakeBinop( BO_lt, $1, $3);
        }
    |   expr GE expr 
        { 
            $$ = TBmakeBinop( BO_ge, $1, $3);
        }
    |   expr GT expr 
        { 
            $$ = TBmakeBinop( BO_gt, $1, $3);
        }
    |   expr EQ expr 
        { 
            $$ = TBmakeBinop( BO_eq, $1, $3);
        }
    |   expr OR expr 
        { 
            $$ = TBmakeBinop( BO_or, $1, $3);
        }
    |   expr AND expr 
        { 
            $$ = TBmakeBinop( BO_and, $1, $3);
        }
    |   expr MINUS expr
        { 
            $$ = TBmakeBinop( BO_add, $1, $3);
        }
    |   expr PLUS expr 
        { 
            $$ = TBmakeBinop( BO_sub, $1, $3);
        }
    |   expr STAR expr
        {
            $$ = TBmakeBinop( BO_mul, $1, $3);
        }
    |   expr SLASH expr
        {
            $$ = TBmakeBinop( BO_div, $1, $3);
        }
    |   expr PERCENT expr 
        { 
            $$ = TBmakeBinop( BO_mod, $1, $3);
        }
    ;

monop:  NEG expr
        {
            $$ = TBmakeMonop( MO_neg, $2);
        }
    |   NOT expr
        {
            $$ = TBmakeMonop( MO_not, $2);
        }
    ;

type: INT      { $$ = T_int; }
     | FLOAT     { $$ = T_float; }
     | BOOL      { $$ = T_bool; }
     | VOID     { $$ = T_void; }
     ;

%%

static int yyerror( char *error)
{
    CTIabort( "line %d, col %d\nError parsing source code: %s\n", 
            global.line, global.col, error);

    return( 0);
}

node *YYparseTree( void)
{
    DBUG_ENTER("YYparseTree");

    yydebug = 1;
    yyparse();

    DBUG_RETURN( parseresult);
}

