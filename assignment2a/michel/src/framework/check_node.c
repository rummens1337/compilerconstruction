
/**
 * @file check_node.c
 *
 * Functions needed by chkm traversal.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: check_node.c.xsl 15657 2007-11-13 13:57:30Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

/**
 * @defgroup check Touch all Tree Functions to catch every node, son and attribute
 *
 * Functions needed by free traversal.
 *
 * @{
 */


#include "check_node.h"
#include "check_attribs.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#define CHKMTRAV( node, info) (node != NULL) ? TRAVdo( node, info) : node
#define CHKMCOND( node, info)                                    \
    ? CHKMTRAV( node, info)                                      \
    : (node)



/*******************************************************************************
 *
 * @fn CHKMpostfun
 *
 * This is the postfun function of the CHKM Traversal  
 *
 ******************************************************************************/
node *
CHKMpostfun (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMpostfun");

  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMassign
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Assign node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMassign (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMassign");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  ASSIGN_LET (arg_node) = CHKMTRAV (ASSIGN_LET (arg_node), arg_info);
  ASSIGN_EXPR (arg_node) = CHKMTRAV (ASSIGN_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMbinop
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMbinop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMbinop");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  BINOP_LEFT (arg_node) = CHKMTRAV (BINOP_LEFT (arg_node), arg_info);
  BINOP_RIGHT (arg_node) = CHKMTRAV (BINOP_RIGHT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMblock
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Block node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMblock (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMblock");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  BLOCK_STMTS (arg_node) = CHKMTRAV (BLOCK_STMTS (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMbool
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Bool node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMbool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMbool");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMdeclarations
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Declarations node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMdeclarations (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMdeclarations");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DECLARATIONS_NEXT (arg_node) =
    CHKMTRAV (DECLARATIONS_NEXT (arg_node), arg_info);
  DECLARATIONS_DECLARATION (arg_node) =
    CHKMTRAV (DECLARATIONS_DECLARATION (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMerror
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Error node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMerror (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMerror");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  ERROR_NEXT (arg_node) = CHKMTRAV (ERROR_NEXT (arg_node), arg_info);
  ERROR_MESSAGE (arg_node) =
    CHKMattribString (ERROR_MESSAGE (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMexprs
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Exprs node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMexprs (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMexprs");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  EXPRS_NEXT (arg_node) = CHKMTRAV (EXPRS_NEXT (arg_node), arg_info);
  EXPRS_EXPR (arg_node) = CHKMTRAV (EXPRS_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfloat
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfloat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfloat");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfor
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node For node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfor (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfor");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FOR_ASSIGN (arg_node) = CHKMTRAV (FOR_ASSIGN (arg_node), arg_info);
  FOR_EXPRS (arg_node) = CHKMTRAV (FOR_EXPRS (arg_node), arg_info);
  FOR_BLOCK (arg_node) = CHKMTRAV (FOR_BLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfunbody
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunBody node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfunbody (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfunbody");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNBODY_VARDECS (arg_node) =
    CHKMTRAV (FUNBODY_VARDECS (arg_node), arg_info);
  FUNBODY_STMTS (arg_node) = CHKMTRAV (FUNBODY_STMTS (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfundec
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfundec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfundec");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNDEC_FUNHEADER (arg_node) =
    CHKMTRAV (FUNDEC_FUNHEADER (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfundef
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfundef (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfundef");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNDEF_FUNHEADER (arg_node) =
    CHKMTRAV (FUNDEF_FUNHEADER (arg_node), arg_info);
  FUNDEF_FUNBODY (arg_node) = CHKMTRAV (FUNDEF_FUNBODY (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfunheader
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunHeader node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfunheader (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfunheader");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNHEADER_ID (arg_node) = CHKMTRAV (FUNHEADER_ID (arg_node), arg_info);
  FUNHEADER_PARAMS (arg_node) =
    CHKMTRAV (FUNHEADER_PARAMS (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMglobaldec
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node GlobalDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMglobaldec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMglobaldec");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  GLOBALDEC_ID (arg_node) = CHKMTRAV (GLOBALDEC_ID (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMglobaldef
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node GlobalDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMglobaldef (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMglobaldef");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  GLOBALDEF_ID (arg_node) = CHKMTRAV (GLOBALDEF_ID (arg_node), arg_info);
  GLOBALDEF_EXPR (arg_node) = CHKMTRAV (GLOBALDEF_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMif
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node If node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMif (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMif");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  IF_EXPR (arg_node) = CHKMTRAV (IF_EXPR (arg_node), arg_info);
  IF_IFBLOCK (arg_node) = CHKMTRAV (IF_IFBLOCK (arg_node), arg_info);
  IF_ELSEBLOCK (arg_node) = CHKMTRAV (IF_ELSEBLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMmodule
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Module node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMmodule (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMmodule");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  MODULE_NEXT (arg_node) = CHKMTRAV (MODULE_NEXT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMmonop
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node MonOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMmonop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMmonop");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  MONOP_EXPR (arg_node) = CHKMTRAV (MONOP_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMnum
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Num node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMnum (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMnum");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMparam
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Param node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMparam (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMparam");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  PARAM_ID (arg_node) = CHKMTRAV (PARAM_ID (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMparams
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Params node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMparams (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMparams");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  PARAMS_PARAM (arg_node) = CHKMTRAV (PARAMS_PARAM (arg_node), arg_info);
  PARAMS_NEXT (arg_node) = CHKMTRAV (PARAMS_NEXT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMprocall
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ProCall node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMprocall (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMprocall");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  PROCALL_ID (arg_node) = CHKMTRAV (PROCALL_ID (arg_node), arg_info);
  PROCALL_EXPRS (arg_node) = CHKMTRAV (PROCALL_EXPRS (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMreturn
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Return node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMreturn (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMreturn");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  RETURN_EXPR (arg_node) = CHKMTRAV (RETURN_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMstmts
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Stmts node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMstmts (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMstmts");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  STMTS_NEXT (arg_node) = CHKMTRAV (STMTS_NEXT (arg_node), arg_info);
  STMTS_STMT (arg_node) = CHKMTRAV (STMTS_STMT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMsymboltableentry
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node SymbolTableEntry node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMsymboltableentry (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMsymboltableentry");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvar
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Var node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvar (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvar");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VAR_NAME (arg_node) = CHKMattribString (VAR_NAME (arg_node), arg_info);
  VAR_DECL (arg_node) = CHKMattribLink (VAR_DECL (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvardec
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node VarDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvardec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvardec");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VARDEC_ID (arg_node) = CHKMTRAV (VARDEC_ID (arg_node), arg_info);
  VARDEC_EXPR (arg_node) = CHKMTRAV (VARDEC_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvardecs
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node VarDecs node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvardecs (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvardecs");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VARDECS_NEXT (arg_node) = CHKMTRAV (VARDECS_NEXT (arg_node), arg_info);
  VARDECS_VARDEC (arg_node) = CHKMTRAV (VARDECS_VARDEC (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvarlet
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node VarLet node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvarlet (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvarlet");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VARLET_NAME (arg_node) =
    CHKMattribString (VARLET_NAME (arg_node), arg_info);
  VARLET_DECL (arg_node) = CHKMattribLink (VARLET_DECL (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMwhile
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node While node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMwhile (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMwhile");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  WHILE_EXPR (arg_node) = CHKMTRAV (WHILE_EXPR (arg_node), arg_info);
  WHILE_BLOCK (arg_node) = CHKMTRAV (WHILE_BLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/**
 * @}
 */
