
/**
 * @file free_node.c
 *
 * Functions needed by free traversal.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: free_node.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

/**
 * @defgroup free Free Tree Functions.
 *
 * Functions needed by free traversal.
 *
 * @{
 */


#include "free.h"
#include "free_node.h"
#include "free_attribs.h"
#include "free_info.h"
#include "tree_basic.h"
#include "traverse.h"
#include "str.h"
#include "memory.h"
#include "dbug.h"

#define FREETRAV( node, info) (node != NULL) ? TRAVdo( node, info) : node
#define FREECOND( node, info)                                    \
  (INFO_FREE_FLAG( info) != arg_node)                            \
    ? FREETRAV( node, info)                                      \
    : (node)


/** <!--******************************************************************-->
 *
 * @fn FREEassign
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Assign node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEassign (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEassign");
  DBUG_PRINT ("FREE", ("Processing node N_assign at " F_PTR, arg_node));
  ASSIGN_LET (arg_node) = FREETRAV (ASSIGN_LET (arg_node), arg_info);
  ASSIGN_EXPR (arg_node) = FREETRAV (ASSIGN_EXPR (arg_node), arg_info);
  result = NULL;
  arg_node->sons.N_assign = MEMfree (arg_node->sons.N_assign);
  arg_node->attribs.N_assign = MEMfree (arg_node->attribs.N_assign);
  DBUG_PRINT ("FREE", ("Processing node N_assign at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEbinop
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEbinop (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEbinop");
  DBUG_PRINT ("FREE", ("Processing node N_binop at " F_PTR, arg_node));
  BINOP_LEFT (arg_node) = FREETRAV (BINOP_LEFT (arg_node), arg_info);
  BINOP_RIGHT (arg_node) = FREETRAV (BINOP_RIGHT (arg_node), arg_info);
  result = NULL;
  arg_node->sons.N_binop = MEMfree (arg_node->sons.N_binop);
  arg_node->attribs.N_binop = MEMfree (arg_node->attribs.N_binop);
  DBUG_PRINT ("FREE", ("Processing node N_binop at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEbool
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Bool node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEbool (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEbool");
  DBUG_PRINT ("FREE", ("Processing node N_bool at " F_PTR, arg_node));
  result = NULL;
  arg_node->sons.N_bool = MEMfree (arg_node->sons.N_bool);
  arg_node->attribs.N_bool = MEMfree (arg_node->attribs.N_bool);
  DBUG_PRINT ("FREE", ("Processing node N_bool at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEerror
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Error node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEerror (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEerror");
  DBUG_PRINT ("FREE", ("Processing node N_error at " F_PTR, arg_node));
  ERROR_NEXT (arg_node) = FREECOND (ERROR_NEXT (arg_node), arg_info);
  ERROR_MESSAGE (arg_node) =
    FREEattribString (ERROR_MESSAGE (arg_node), arg_node);
  result = ERROR_NEXT (arg_node);
  arg_node->sons.N_error = MEMfree (arg_node->sons.N_error);
  arg_node->attribs.N_error = MEMfree (arg_node->attribs.N_error);
  DBUG_PRINT ("FREE", ("Processing node N_error at " F_PTR, arg_node));
  arg_node = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEfloat
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEfloat (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEfloat");
  DBUG_PRINT ("FREE", ("Processing node N_float at " F_PTR, arg_node));
  result = NULL;
  arg_node->sons.N_float = MEMfree (arg_node->sons.N_float);
  arg_node->attribs.N_float = MEMfree (arg_node->attribs.N_float);
  DBUG_PRINT ("FREE", ("Processing node N_float at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEmodule
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Module node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEmodule (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEmodule");
  DBUG_PRINT ("FREE", ("Processing node N_module at " F_PTR, arg_node));
  MODULE_LEFT (arg_node) = FREETRAV (MODULE_LEFT (arg_node), arg_info);
  MODULE_RIGHT (arg_node) = FREETRAV (MODULE_RIGHT (arg_node), arg_info);
  result = NULL;
  arg_node->sons.N_module = MEMfree (arg_node->sons.N_module);
  arg_node->attribs.N_module = MEMfree (arg_node->attribs.N_module);
  DBUG_PRINT ("FREE", ("Processing node N_module at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEnum
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Num node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEnum (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEnum");
  DBUG_PRINT ("FREE", ("Processing node N_num at " F_PTR, arg_node));
  result = NULL;
  arg_node->sons.N_num = MEMfree (arg_node->sons.N_num);
  arg_node->attribs.N_num = MEMfree (arg_node->attribs.N_num);
  DBUG_PRINT ("FREE", ("Processing node N_num at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEstmts
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Stmts node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEstmts (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEstmts");
  DBUG_PRINT ("FREE", ("Processing node N_stmts at " F_PTR, arg_node));
  STMTS_NEXT (arg_node) = FREECOND (STMTS_NEXT (arg_node), arg_info);
  STMTS_STMT (arg_node) = FREETRAV (STMTS_STMT (arg_node), arg_info);
  result = STMTS_NEXT (arg_node);
  arg_node->sons.N_stmts = MEMfree (arg_node->sons.N_stmts);
  arg_node->attribs.N_stmts = MEMfree (arg_node->attribs.N_stmts);
  DBUG_PRINT ("FREE", ("Processing node N_stmts at " F_PTR, arg_node));
  arg_node = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEsymboltableentry
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node SymbolTableEntry node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEsymboltableentry (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEsymboltableentry");
  DBUG_PRINT ("FREE",
	      ("Processing node N_symboltableentry at " F_PTR, arg_node));
  result = NULL;
  arg_node->sons.N_symboltableentry =
    MEMfree (arg_node->sons.N_symboltableentry);
  arg_node->attribs.N_symboltableentry =
    MEMfree (arg_node->attribs.N_symboltableentry);
  DBUG_PRINT ("FREE",
	      ("Processing node N_symboltableentry at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEvar
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Var node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEvar (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEvar");
  DBUG_PRINT ("FREE", ("Processing node N_var at " F_PTR, arg_node));
  VAR_NAME (arg_node) = FREEattribString (VAR_NAME (arg_node), arg_node);
  VAR_DECL (arg_node) = FREEattribLink (VAR_DECL (arg_node), arg_node);
  result = NULL;
  arg_node->sons.N_var = MEMfree (arg_node->sons.N_var);
  arg_node->attribs.N_var = MEMfree (arg_node->attribs.N_var);
  DBUG_PRINT ("FREE", ("Processing node N_var at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn FREEvarlet
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node VarLet node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
FREEvarlet (node * arg_node, info * arg_info)
{
  node *result = NULL;
  DBUG_ENTER ("FREEvarlet");
  DBUG_PRINT ("FREE", ("Processing node N_varlet at " F_PTR, arg_node));
  VARLET_NAME (arg_node) =
    FREEattribString (VARLET_NAME (arg_node), arg_node);
  VARLET_DECL (arg_node) = FREEattribLink (VARLET_DECL (arg_node), arg_node);
  result = NULL;
  arg_node->sons.N_varlet = MEMfree (arg_node->sons.N_varlet);
  arg_node->attribs.N_varlet = MEMfree (arg_node->attribs.N_varlet);
  DBUG_PRINT ("FREE", ("Processing node N_varlet at " F_PTR, arg_node));
  result = MEMfree (arg_node);
  DBUG_RETURN (result);
}

/**
 * @}
 */
