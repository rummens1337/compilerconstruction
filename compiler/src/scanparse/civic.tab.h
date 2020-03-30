/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PARENTHESIS_L = 258,
    PARENTHESIS_R = 259,
    CURLY_L = 260,
    CURLY_R = 261,
    BRACKET_L = 262,
    BRACKET_R = 263,
    COMMA = 264,
    SEMICOLON = 265,
    MINUS = 266,
    PLUS = 267,
    STAR = 268,
    SLASH = 269,
    PERCENT = 270,
    LE = 271,
    LT = 272,
    GE = 273,
    GT = 274,
    EQ = 275,
    NE = 276,
    OR = 277,
    AND = 278,
    LET = 279,
    NEG = 280,
    INT = 281,
    FLOAT = 282,
    BOOL = 283,
    VOID = 284,
    TRUEVAL = 285,
    FALSEVAL = 286,
    EXTERN = 287,
    EXPORT = 288,
    RETURN = 289,
    IF = 290,
    ELSE = 291,
    DO = 292,
    WHILE = 293,
    FOR = 294,
    NUM = 295,
    FLOATNUM = 296,
    ID = 297
  };
#endif
/* Tokens.  */
#define PARENTHESIS_L 258
#define PARENTHESIS_R 259
#define CURLY_L 260
#define CURLY_R 261
#define BRACKET_L 262
#define BRACKET_R 263
#define COMMA 264
#define SEMICOLON 265
#define MINUS 266
#define PLUS 267
#define STAR 268
#define SLASH 269
#define PERCENT 270
#define LE 271
#define LT 272
#define GE 273
#define GT 274
#define EQ 275
#define NE 276
#define OR 277
#define AND 278
#define LET 279
#define NEG 280
#define INT 281
#define FLOAT 282
#define BOOL 283
#define VOID 284
#define TRUEVAL 285
#define FALSEVAL 286
#define EXTERN 287
#define EXPORT 288
#define RETURN 289
#define IF 290
#define ELSE 291
#define DO 292
#define WHILE 293
#define FOR 294
#define NUM 295
#define FLOATNUM 296
#define ID 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 28 "src/scanparse/civic.y" /* yacc.c:1909  */

 nodetype            nodetype;
 char               *id;
 int                 cint;
 float               cflt;
 binop               cbinop;
 monop               cmonop;
 type                ctype;
 node               *node;

#line 149 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
