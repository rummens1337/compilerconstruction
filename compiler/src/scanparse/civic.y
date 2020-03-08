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

#define YYDEBUG 1

static node *parseresult = NULL;
extern int yylex();
static int yyerror( char *errname);

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

%type <node> intval floatval boolval constant expr binop
%type <node> stmts stmt assign varlet program
%type <node> return exprs funcall opprec5 opprec6 opprec4 opprec3 opprec2 opprec1

%type <ctype> type

%start program

%%

program: stmts 
         {
           parseresult = $1;
         }
         ;

stmts: stmt stmts
        {
          $$ = TBmakeStmts( $1, $2);
        }
      | stmt
        {
          $$ = TBmakeStmts( $1, NULL);
        }
        ;

stmt: assign
      {
        $$ = $1;
      }
    | return
      {
        $$ = $1;
      }
    | funcall
      {
        $$ = $1;
      }
      ;

return: RETURN expr SEMICOLON
        {
          $$ = TBmakeReturn( $2);
        }
        ;

assign: varlet LET expr SEMICOLON
        {
          $$ = TBmakeAssign( $1, $3);
        }
        ;

funcall: ID PARENTHESIS_L exprs PARENTHESIS_R
        {
          $$ = TBmakeFuncall( STRcpy( $1), NULL, $3);
        }
        | ID PARENTHESIS_L PARENTHESIS_R
        {
          $$ = TBmakeFuncall( STRcpy( $1), NULL, NULL);
        }
        ;

varlet: ID
        {
          $$ = TBmakeVarlet( STRcpy( $1), NULL, NULL);
        }
        ;

exprs: expr COMMA exprs
      {
        $$ = TBmakeExprs($1, $3);
      }
      | expr
      {
        $$ = TBmakeExprs($1, NULL);
      }
      ;

expr: 
      PARENTHESIS_L type PARENTHESIS_R expr
      {
        $$ = TBmakeCast( $2, $4);
      }
    | opprec6
    | ID
      {
        $$ = TBmakeVar( STRcpy( $1), NULL, NULL);
      }
    ;

opprec6: opprec5
        | opprec6 AND opprec5
          {
            TBmakeBinop(BO_and, $1, $3);
          }
        ;
opprec5: opprec4
        | opprec5 OR constant
          {
            TBmakeBinop(BO_or, $1, $3);
          }
          ;
opprec4: constant
          {
            $$ = $1;
          }
        | opprec5 MINUS constant
          {
            TBmakeBinop(BO_sub, $1, $3);
          }
          ;

constant: floatval
          {
            $$ = $1;
          }
        | intval
          {
            $$ = $1;
          }
        | boolval
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
       | FALSEVAL
         {
           $$ = TBmakeBool( FALSE);
         }
       ;

// binop: PLUS      { $$ = BO_add; }
//      | MINUS     { $$ = BO_sub; }
//      | STAR      { $$ = BO_mul; }
//      | SLASH     { $$ = BO_div; }
//      | PERCENT   { $$ = BO_mod; }
//      | LE        { $$ = BO_le; }
//      | LT        { $$ = BO_lt; }
//      | GE        { $$ = BO_ge; }
//      | GT        { $$ = BO_gt; }
//      | EQ        { $$ = BO_eq; }
//      | OR        { $$ = BO_or; }
//      | AND       { $$ = BO_and; }
//      ;

// monop: NOT       { $$ = MO_not; }
//      | NEG       { $$ = MO_neg; }
//      ;

type:  INT     { $$ = T_int; }
     | FLOAT   { $$ = T_float; }
     | BOOL    { $$ = T_bool; }
     | VOID    { $$ = T_void; }
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

