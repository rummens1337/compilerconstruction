
/**
 * @file node_basic.c
 *
 * Functions to allocate node structures
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: node_basic.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#include "node_basic.h"
#include "tree_basic.h"
#include "memory.h"
#include "dbug.h"
#include "globals.h"
#include "ctinfo.h"

static node *
MakeEmptyNode ()
{
  node *result;

  DBUG_ENTER ("MakeEmptyNode");

  result = (node *) MEMmalloc (sizeof (node));

  NODE_LINE (result) = global.line;
  NODE_COL (result) = global.col;

  DBUG_RETURN (result);
}



/*****************************************************************************
 * N_Module :
 *****************************************************************************/

node *
TBmakeModule (int Add, int Sub, int Div, int Mul, int Mod, node * Next)
{
  node *this;
  DBUG_ENTER ("TBmakeModule");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_module;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_module = MEMmalloc (sizeof (struct SONS_N_MODULE));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_module = MEMmalloc (sizeof (struct ATTRIBS_N_MODULE));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_module;
  DBUG_PRINT ("MAKE", ("assigning son Next initial value: %s ", Next));
  MODULE_NEXT (this) = Next;
  MODULE_ADD (this) = Add;
  MODULE_SUB (this) = Sub;
  MODULE_DIV (this) = Div;
  MODULE_MUL (this) = Mul;
  MODULE_MOD (this) = Mod;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
  if ((MODULE_NEXT (this) != NULL)
      && (NODE_TYPE (MODULE_NEXT (this)) != N_stmts))
    {
      CTIwarn ("Field Next of node N_Module has non-allowed target node.");
    }
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_Stmts :
 *****************************************************************************/

node *
TBmakeStmts (node * Stmt, node * Next)
{
  node *this;
  DBUG_ENTER ("TBmakeStmts");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_stmts;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_stmts = MEMmalloc (sizeof (struct SONS_N_STMTS));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_stmts = MEMmalloc (sizeof (struct ATTRIBS_N_STMTS));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_stmts;
  DBUG_PRINT ("MAKE", ("assigning son Stmt initial value: %s ", Stmt));
  STMTS_STMT (this) = Stmt;
  DBUG_PRINT ("MAKE", ("assigning son Next initial value: %s ", Next));
  STMTS_NEXT (this) = Next;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
  if ((STMTS_STMT (this) != NULL)
      && (NODE_TYPE (STMTS_STMT (this)) != N_assign))
    {
      CTIwarn ("Field Stmt of node N_Stmts has non-allowed target node.");
    }
  if ((STMTS_NEXT (this) != NULL)
      && (NODE_TYPE (STMTS_NEXT (this)) != N_stmts))
    {
      CTIwarn ("Field Next of node N_Stmts has non-allowed target node.");
    }
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_Assign :
 *****************************************************************************/

node *
TBmakeAssign (node * Let, node * Expr)
{
  node *this;
  DBUG_ENTER ("TBmakeAssign");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_assign;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_assign = MEMmalloc (sizeof (struct SONS_N_ASSIGN));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_assign = MEMmalloc (sizeof (struct ATTRIBS_N_ASSIGN));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_assign;
  DBUG_PRINT ("MAKE", ("assigning son Let initial value: %s ", Let));
  ASSIGN_LET (this) = Let;
  DBUG_PRINT ("MAKE", ("assigning son Expr initial value: %s ", Expr));
  ASSIGN_EXPR (this) = Expr;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
  if ((ASSIGN_LET (this) != NULL)
      && (NODE_TYPE (ASSIGN_LET (this)) != N_varlet))
    {
      CTIwarn ("Field Let of node N_Assign has non-allowed target node.");
    }
  if ((ASSIGN_EXPR (this) != NULL)
      && (NODE_TYPE (ASSIGN_EXPR (this)) != N_binop)
      && (NODE_TYPE (ASSIGN_EXPR (this)) != N_var)
      && (NODE_TYPE (ASSIGN_EXPR (this)) != N_num)
      && (NODE_TYPE (ASSIGN_EXPR (this)) != N_float)
      && (NODE_TYPE (ASSIGN_EXPR (this)) != N_bool))
    {
      CTIwarn ("Field Expr of node N_Assign has non-allowed target node.");
    }
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_BinOp :
 *****************************************************************************/

node *
TBmakeBinop (binop Op, node * Left, node * Right)
{
  node *this;
  DBUG_ENTER ("TBmakeBinop");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_binop;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_binop = MEMmalloc (sizeof (struct SONS_N_BINOP));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_binop = MEMmalloc (sizeof (struct ATTRIBS_N_BINOP));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_binop;
  DBUG_PRINT ("MAKE", ("assigning son Left initial value: %s ", Left));
  BINOP_LEFT (this) = Left;
  DBUG_PRINT ("MAKE", ("assigning son Right initial value: %s ", Right));
  BINOP_RIGHT (this) = Right;
  BINOP_OP (this) = Op;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
  if ((BINOP_LEFT (this) != NULL)
      && (NODE_TYPE (BINOP_LEFT (this)) != N_binop)
      && (NODE_TYPE (BINOP_LEFT (this)) != N_var)
      && (NODE_TYPE (BINOP_LEFT (this)) != N_num)
      && (NODE_TYPE (BINOP_LEFT (this)) != N_float)
      && (NODE_TYPE (BINOP_LEFT (this)) != N_bool))
    {
      CTIwarn ("Field Left of node N_BinOp has non-allowed target node.");
    }
  if ((BINOP_RIGHT (this) != NULL)
      && (NODE_TYPE (BINOP_RIGHT (this)) != N_binop)
      && (NODE_TYPE (BINOP_RIGHT (this)) != N_var)
      && (NODE_TYPE (BINOP_RIGHT (this)) != N_num)
      && (NODE_TYPE (BINOP_RIGHT (this)) != N_float)
      && (NODE_TYPE (BINOP_RIGHT (this)) != N_bool))
    {
      CTIwarn ("Field Right of node N_BinOp has non-allowed target node.");
    }
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_VarLet :
 *****************************************************************************/

node *
TBmakeVarlet (char *Name)
{
  node *this;
  DBUG_ENTER ("TBmakeVarlet");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_varlet;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_varlet = MEMmalloc (sizeof (struct SONS_N_VARLET));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_varlet = MEMmalloc (sizeof (struct ATTRIBS_N_VARLET));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_varlet;
  VARLET_NAME (this) = Name;
  VARLET_DECL (this) = NULL;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_Var :
 *****************************************************************************/

node *
TBmakeVar (char *Name)
{
  node *this;
  DBUG_ENTER ("TBmakeVar");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_var;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_var = MEMmalloc (sizeof (struct SONS_N_VAR));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_var = MEMmalloc (sizeof (struct ATTRIBS_N_VAR));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_var;
  VAR_NAME (this) = Name;
  VAR_DECL (this) = NULL;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_Num :
 *****************************************************************************/

node *
TBmakeNum (int Value)
{
  node *this;
  DBUG_ENTER ("TBmakeNum");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_num;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_num = MEMmalloc (sizeof (struct SONS_N_NUM));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_num = MEMmalloc (sizeof (struct ATTRIBS_N_NUM));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_num;
  NUM_VALUE (this) = Value;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_Float :
 *****************************************************************************/

node *
TBmakeFloat (float Value)
{
  node *this;
  DBUG_ENTER ("TBmakeFloat");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_float;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_float = MEMmalloc (sizeof (struct SONS_N_FLOAT));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_float = MEMmalloc (sizeof (struct ATTRIBS_N_FLOAT));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_float;
  FLOAT_VALUE (this) = Value;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_Bool :
 *****************************************************************************/

node *
TBmakeBool (bool Value)
{
  node *this;
  DBUG_ENTER ("TBmakeBool");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_bool;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_bool = MEMmalloc (sizeof (struct SONS_N_BOOL));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_bool = MEMmalloc (sizeof (struct ATTRIBS_N_BOOL));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_bool;
  BOOL_VALUE (this) = Value;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_SymbolTableEntry :
 *****************************************************************************/

node *
TBmakeSymboltableentry ()
{
  node *this;
  DBUG_ENTER ("TBmakeSymboltableentry");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_symboltableentry;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_symboltableentry =
    MEMmalloc (sizeof (struct SONS_N_SYMBOLTABLEENTRY));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_symboltableentry =
    MEMmalloc (sizeof (struct ATTRIBS_N_SYMBOLTABLEENTRY));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_symboltableentry;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

/*****************************************************************************
 * N_Error :
 *****************************************************************************/

node *
TBmakeError (char *message, compiler_phase_t anyphase, node * Next)
{
  node *this;
  DBUG_ENTER ("TBmakeError");
  DBUG_PRINT ("MAKE", ("allocating node structure"));
  this = MakeEmptyNode ();
  NODE_TYPE (this) = N_error;
  DBUG_PRINT ("MAKE", ("address: %s ", this));
  DBUG_PRINT ("MAKE", ("allocating sons structure"));
  this->sons.N_error = MEMmalloc (sizeof (struct SONS_N_ERROR));
  DBUG_PRINT ("MAKE", ("allocating attrib structure"));
  this->attribs.N_error = MEMmalloc (sizeof (struct ATTRIBS_N_ERROR));
  DBUG_PRINT ("MAKE", ("setting node type"));
  NODE_TYPE (this) = N_error;
  DBUG_PRINT ("MAKE", ("assigning son Next initial value: %s ", Next));
  ERROR_NEXT (this) = Next;
  ERROR_MESSAGE (this) = message;
  ERROR_ANYPHASE (this) = anyphase;
#ifndef DBUG_OFF
  DBUG_PRINT ("MAKE", ("doing son target checks"));
  if ((ERROR_NEXT (this) != NULL)
      && (NODE_TYPE (ERROR_NEXT (this)) != N_error))
    {
      CTIwarn ("Field Next of node N_Error has non-allowed target node.");
    }
#endif /* DBUG_OFF */
  DBUG_RETURN (this);
}

  /* end of file */
