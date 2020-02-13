
/**
 * @file eval.c
 *
 * Functions needed for compiling
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: free_node.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

/**
 * @defgroup comp Compilation functions.
 *
 * Functions needed for compiling.
 *
 * @{
 */


#include "eval.h"
#include "traverse.h"
#include "dbug.h"


/** <!--******************************************************************-->
 *
 * @fn EVALassign
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
EVALassign (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALassign");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALbinop
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
EVALbinop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALbinop");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALbool
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
EVALbool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALbool");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALerror
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
EVALerror (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALerror");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfloat
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
EVALfloat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfloat");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALnum
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
EVALnum (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALnum");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALstmts
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
EVALstmts (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALstmts");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALsymboltableentry
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
EVALsymboltableentry (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALsymboltableentry");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvar
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
EVALvar (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvar");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvarlet
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
EVALvarlet (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvarlet");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/**
 * @}
 */
