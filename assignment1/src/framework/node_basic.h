
/**
 * @file node_basic.h
 *
 * Functions to allocate node structures
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: node_basic.h.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_NODE_BASIC_H_
#define _SAC_NODE_BASIC_H_

#include "types.h"


#define _SAC_AST_VERSION_ "0.9"


/*****************************************************************************
 * macros and functions for N_module
 *****************************************************************************/

#define MODULE_LEFT( n) ((n)->sons.N_module->Left)
#define MODULE_RIGHT( n) ((n)->sons.N_module->Right)
#define MODULE_ADD( n) ((n)->attribs.N_module->Add)
#define MODULE_SUB( n) ((n)->attribs.N_module->Sub)
#define MODULE_DIV( n) ((n)->attribs.N_module->Div)
#define MODULE_MUL( n) ((n)->attribs.N_module->Mul)
#define MODULE_MOD( n) ((n)->attribs.N_module->Mod)
extern node *TBmakeModule (int Add, int Sub, int Div, int Mul, int Mod, node * Left, node * Right);

/*****************************************************************************
 * macros and functions for N_stmts
 *****************************************************************************/

#define STMTS_STMT( n) ((n)->sons.N_stmts->Stmt)
#define STMTS_NEXT( n) ((n)->sons.N_stmts->Next)
extern node *TBmakeStmts (node * Stmt, node * Next);

/*****************************************************************************
 * macros and functions for N_assign
 *****************************************************************************/

#define ASSIGN_LET( n) ((n)->sons.N_assign->Let)
#define ASSIGN_EXPR( n) ((n)->sons.N_assign->Expr)
extern node *TBmakeAssign (node * Let, node * Expr);

/*****************************************************************************
 * macros and functions for N_binop
 *****************************************************************************/

#define BINOP_LEFT( n) ((n)->sons.N_binop->Left)
#define BINOP_RIGHT( n) ((n)->sons.N_binop->Right)
#define BINOP_OP( n) ((n)->attribs.N_binop->Op)
extern node *TBmakeBinop (binop Op, node * Left, node * Right);

/*****************************************************************************
 * macros and functions for N_varlet
 *****************************************************************************/

#define VARLET_NAME( n) ((n)->attribs.N_varlet->Name)
#define VARLET_DECL( n) ((n)->attribs.N_varlet->Decl)
extern node *TBmakeVarlet (char *Name);

/*****************************************************************************
 * macros and functions for N_var
 *****************************************************************************/

#define VAR_NAME( n) ((n)->attribs.N_var->Name)
#define VAR_DECL( n) ((n)->attribs.N_var->Decl)
extern node *TBmakeVar (char *Name);

/*****************************************************************************
 * macros and functions for N_num
 *****************************************************************************/

#define NUM_VALUE( n) ((n)->attribs.N_num->Value)
extern node *TBmakeNum (int Value);

/*****************************************************************************
 * macros and functions for N_float
 *****************************************************************************/

#define FLOAT_VALUE( n) ((n)->attribs.N_float->Value)
extern node *TBmakeFloat (float Value);

/*****************************************************************************
 * macros and functions for N_bool
 *****************************************************************************/

#define BOOL_VALUE( n) ((n)->attribs.N_bool->Value)
extern node *TBmakeBool (bool Value);

/*****************************************************************************
 * macros and functions for N_symboltableentry
 *****************************************************************************/

extern node *TBmakeSymboltableentry ();

/*****************************************************************************
 * macros and functions for N_error
 *****************************************************************************/

#define ERROR_NEXT( n) ((n)->sons.N_error->Next)
#define ERROR_MESSAGE( n) ((n)->attribs.N_error->message)
#define ERROR_ANYPHASE( n) ((n)->attribs.N_error->anyphase)
extern node *TBmakeError (char *message, compiler_phase_t anyphase,
			  node * Next);

#endif /* _SAC_NODE_BASIC_H_ */
