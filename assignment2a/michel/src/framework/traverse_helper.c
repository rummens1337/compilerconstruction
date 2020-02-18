
/**
 * @file traverse_helper.c
 *
 * Defines the helper function needed by the traversal system
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: traverse_helper.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */


#include "traverse_helper.h"
#include "dbug.h"
#include "tree_basic.h"
#include "traverse.h"

#define TRAV( son, info)    if (son != NULL) { son = TRAVdo( son, info); }

node *
TRAVnone (node * arg_node, info * arg_info)
{
  return (arg_node);
}

node *
TRAVerror (node * arg_node, info * arg_info)
{
  DBUG_ASSERT ((FALSE), "Illegal node type found.");

  return (arg_node);
}

node *
TRAVsons (node * arg_node, info * arg_info)
{
  switch (NODE_TYPE (arg_node))
    {
    case N_declarations:
      TRAV (DECLARATIONS_DECLARATION (arg_node), arg_info);
      TRAV (DECLARATIONS_NEXT (arg_node), arg_info);
      break;
    case N_fundec:
      TRAV (FUNDEC_FUNHEADER (arg_node), arg_info);
      break;
    case N_fundef:
      TRAV (FUNDEF_FUNHEADER (arg_node), arg_info);
      TRAV (FUNDEF_FUNBODY (arg_node), arg_info);
      break;
    case N_funheader:
      TRAV (FUNHEADER_ID (arg_node), arg_info);
      TRAV (FUNHEADER_PARAMS (arg_node), arg_info);
      break;
    case N_param:
      TRAV (PARAM_ID (arg_node), arg_info);
      break;
    case N_params:
      TRAV (PARAMS_PARAM (arg_node), arg_info);
      TRAV (PARAMS_NEXT (arg_node), arg_info);
      break;
    case N_globaldec:
      TRAV (GLOBALDEC_ID (arg_node), arg_info);
      break;
    case N_globaldef:
      TRAV (GLOBALDEF_ID (arg_node), arg_info);
      TRAV (GLOBALDEF_EXPR (arg_node), arg_info);
      break;
    case N_funbody:
      TRAV (FUNBODY_VARDECS (arg_node), arg_info);
      TRAV (FUNBODY_STMTS (arg_node), arg_info);
      break;
    case N_vardec:
      TRAV (VARDEC_ID (arg_node), arg_info);
      TRAV (VARDEC_EXPR (arg_node), arg_info);
      break;
    case N_vardecs:
      TRAV (VARDECS_VARDEC (arg_node), arg_info);
      TRAV (VARDECS_NEXT (arg_node), arg_info);
      break;
    case N_procall:
      TRAV (PROCALL_ID (arg_node), arg_info);
      TRAV (PROCALL_EXPRS (arg_node), arg_info);
      break;
    case N_block:
      TRAV (BLOCK_STMTS (arg_node), arg_info);
      break;
    case N_if:
      TRAV (IF_EXPR (arg_node), arg_info);
      TRAV (IF_IFBLOCK (arg_node), arg_info);
      TRAV (IF_ELSEBLOCK (arg_node), arg_info);
      break;
    case N_while:
      TRAV (WHILE_EXPR (arg_node), arg_info);
      TRAV (WHILE_BLOCK (arg_node), arg_info);
      break;
    case N_for:
      TRAV (FOR_ASSIGN (arg_node), arg_info);
      TRAV (FOR_EXPRS (arg_node), arg_info);
      TRAV (FOR_BLOCK (arg_node), arg_info);
      break;
    case N_return:
      TRAV (RETURN_EXPR (arg_node), arg_info);
      break;
    case N_monop:
      TRAV (MONOP_EXPR (arg_node), arg_info);
      break;
    case N_module:
      TRAV (MODULE_NEXT (arg_node), arg_info);
      break;
    case N_exprs:
      TRAV (EXPRS_EXPR (arg_node), arg_info);
      TRAV (EXPRS_NEXT (arg_node), arg_info);
      break;
    case N_stmts:
      TRAV (STMTS_STMT (arg_node), arg_info);
      TRAV (STMTS_NEXT (arg_node), arg_info);
      break;
    case N_assign:
      TRAV (ASSIGN_LET (arg_node), arg_info);
      TRAV (ASSIGN_EXPR (arg_node), arg_info);
      break;
    case N_binop:
      TRAV (BINOP_LEFT (arg_node), arg_info);
      TRAV (BINOP_RIGHT (arg_node), arg_info);
      break;
    case N_varlet:
      break;
    case N_var:
      break;
    case N_num:
      break;
    case N_float:
      break;
    case N_bool:
      break;
    case N_symboltableentry:
      break;
    case N_error:
      TRAV (ERROR_NEXT (arg_node), arg_info);
      break;
    default:
      DBUG_ASSERT ((FALSE), "Illegal nodetype found!");
      break;
    }

  return (arg_node);
}

int
TRAVnumSons (node * node)
{
  int result = 0;

  DBUG_ENTER ("TRAVnumSons");

  switch (NODE_TYPE (node))
    {
    case N_declarations:
      result = 2;
      break;
    case N_fundec:
      result = 1;
      break;
    case N_fundef:
      result = 2;
      break;
    case N_funheader:
      result = 2;
      break;
    case N_param:
      result = 1;
      break;
    case N_params:
      result = 2;
      break;
    case N_globaldec:
      result = 1;
      break;
    case N_globaldef:
      result = 2;
      break;
    case N_funbody:
      result = 2;
      break;
    case N_vardec:
      result = 2;
      break;
    case N_vardecs:
      result = 2;
      break;
    case N_procall:
      result = 2;
      break;
    case N_block:
      result = 1;
      break;
    case N_if:
      result = 3;
      break;
    case N_while:
      result = 2;
      break;
    case N_for:
      result = 3;
      break;
    case N_return:
      result = 1;
      break;
    case N_monop:
      result = 1;
      break;
    case N_module:
      result = 1;
      break;
    case N_exprs:
      result = 2;
      break;
    case N_stmts:
      result = 2;
      break;
    case N_assign:
      result = 2;
      break;
    case N_binop:
      result = 2;
      break;
    case N_varlet:
      result = 0;
      break;
    case N_var:
      result = 0;
      break;
    case N_num:
      result = 0;
      break;
    case N_float:
      result = 0;
      break;
    case N_bool:
      result = 0;
      break;
    case N_symboltableentry:
      result = 0;
      break;
    case N_error:
      result = 1;
      break;
    default:
      DBUG_ASSERT ((FALSE), "Illegal nodetype found!");
      break;
    }

  DBUG_RETURN (result);
}

node *
TRAVgetSon (int no, node * parent)
{
  node *result = NULL;

  DBUG_ENTER ("TRAVgetSon");

  switch (NODE_TYPE (parent))
    {
    case N_declarations:
      switch (no)
	{
	case 0:
	  result = DECLARATIONS_DECLARATION (parent);
	  break;
	case 1:
	  result = DECLARATIONS_NEXT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_fundec:
      switch (no)
	{
	case 0:
	  result = FUNDEC_FUNHEADER (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_fundef:
      switch (no)
	{
	case 0:
	  result = FUNDEF_FUNHEADER (parent);
	  break;
	case 1:
	  result = FUNDEF_FUNBODY (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_funheader:
      switch (no)
	{
	case 0:
	  result = FUNHEADER_ID (parent);
	  break;
	case 1:
	  result = FUNHEADER_PARAMS (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_param:
      switch (no)
	{
	case 0:
	  result = PARAM_ID (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_params:
      switch (no)
	{
	case 0:
	  result = PARAMS_PARAM (parent);
	  break;
	case 1:
	  result = PARAMS_NEXT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_globaldec:
      switch (no)
	{
	case 0:
	  result = GLOBALDEC_ID (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_globaldef:
      switch (no)
	{
	case 0:
	  result = GLOBALDEF_ID (parent);
	  break;
	case 1:
	  result = GLOBALDEF_EXPR (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_funbody:
      switch (no)
	{
	case 0:
	  result = FUNBODY_VARDECS (parent);
	  break;
	case 1:
	  result = FUNBODY_STMTS (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_vardec:
      switch (no)
	{
	case 0:
	  result = VARDEC_ID (parent);
	  break;
	case 1:
	  result = VARDEC_EXPR (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_vardecs:
      switch (no)
	{
	case 0:
	  result = VARDECS_VARDEC (parent);
	  break;
	case 1:
	  result = VARDECS_NEXT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_procall:
      switch (no)
	{
	case 0:
	  result = PROCALL_ID (parent);
	  break;
	case 1:
	  result = PROCALL_EXPRS (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_block:
      switch (no)
	{
	case 0:
	  result = BLOCK_STMTS (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_if:
      switch (no)
	{
	case 0:
	  result = IF_EXPR (parent);
	  break;
	case 1:
	  result = IF_IFBLOCK (parent);
	  break;
	case 2:
	  result = IF_ELSEBLOCK (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_while:
      switch (no)
	{
	case 0:
	  result = WHILE_EXPR (parent);
	  break;
	case 1:
	  result = WHILE_BLOCK (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_for:
      switch (no)
	{
	case 0:
	  result = FOR_ASSIGN (parent);
	  break;
	case 1:
	  result = FOR_EXPRS (parent);
	  break;
	case 2:
	  result = FOR_BLOCK (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_return:
      switch (no)
	{
	case 0:
	  result = RETURN_EXPR (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_monop:
      switch (no)
	{
	case 0:
	  result = MONOP_EXPR (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_module:
      switch (no)
	{
	case 0:
	  result = MODULE_NEXT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_exprs:
      switch (no)
	{
	case 0:
	  result = EXPRS_EXPR (parent);
	  break;
	case 1:
	  result = EXPRS_NEXT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_stmts:
      switch (no)
	{
	case 0:
	  result = STMTS_STMT (parent);
	  break;
	case 1:
	  result = STMTS_NEXT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_assign:
      switch (no)
	{
	case 0:
	  result = ASSIGN_LET (parent);
	  break;
	case 1:
	  result = ASSIGN_EXPR (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_binop:
      switch (no)
	{
	case 0:
	  result = BINOP_LEFT (parent);
	  break;
	case 1:
	  result = BINOP_RIGHT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_varlet:
      switch (no)
	{
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_var:
      switch (no)
	{
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_num:
      switch (no)
	{
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_float:
      switch (no)
	{
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_bool:
      switch (no)
	{
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_symboltableentry:
      switch (no)
	{
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    case N_error:
      switch (no)
	{
	case 0:
	  result = ERROR_NEXT (parent);
	  break;
	default:
	  DBUG_ASSERT ((FALSE), "index out of range!");
	  break;
	}
      break;
    default:
      DBUG_ASSERT ((FALSE), "Illegal nodetype found!");
      break;
    }

  DBUG_RETURN (result);
}
