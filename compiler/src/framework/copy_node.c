
/**
 * @file copy_node.c
 *
 * Functions needed by copy traversal.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * copy_node.c.xsl.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

/**
 * @defgroup copy Copy Tree Functions.
 *
 * Functions needed by copy traversal.
 *
 * @{
 */


#include "copy_node.h"
#include "copy_info.h"
#include "traverse.h"
#include "dbug.h"
#include "tree_basic.h"
#include "str.h"
#include "lookup_table.h"


#define COPYTRAV( node, info) (node != NULL) ? TRAVdo( node, info) : node


/** <!--******************************************************************-->
 *
 * @fn COPYarrexpr
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node ArrExpr node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYarrexpr (node * arg_node, info * arg_info)
{
  node *result = TBmakeArrexpr (NULL);
  DBUG_ENTER ("COPYarrexpr");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  ARREXPR_EXPRS (result) = COPYTRAV (ARREXPR_EXPRS (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYassign
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Assign node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYassign (node * arg_node, info * arg_info)
{
  node *result = TBmakeAssign (NULL, NULL);
  DBUG_ENTER ("COPYassign");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  ASSIGN_LET (result) = COPYTRAV (ASSIGN_LET (arg_node), arg_info);
  ASSIGN_EXPR (result) = COPYTRAV (ASSIGN_EXPR (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYbinop
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYbinop (node * arg_node, info * arg_info)
{
  node *result = TBmakeBinop (BO_unknown, NULL, NULL);
  DBUG_ENTER ("COPYbinop");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  BINOP_OP (result) = BINOP_OP (arg_node);
  /* Copy sons */
  BINOP_LEFT (result) = COPYTRAV (BINOP_LEFT (arg_node), arg_info);
  BINOP_RIGHT (result) = COPYTRAV (BINOP_RIGHT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYbool
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Bool node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYbool (node * arg_node, info * arg_info)
{
  node *result = TBmakeBool (FALSE);
  DBUG_ENTER ("COPYbool");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  BOOL_VALUE (result) = BOOL_VALUE (arg_node);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYcast
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Cast node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYcast (node * arg_node, info * arg_info)
{
  node *result = TBmakeCast (T_unknown, NULL);
  DBUG_ENTER ("COPYcast");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  CAST_TYPE (result) = CAST_TYPE (arg_node);
  /* Copy sons */
  CAST_EXPR (result) = COPYTRAV (CAST_EXPR (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYdecls
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Decls node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYdecls (node * arg_node, info * arg_info)
{
  node *result = TBmakeDecls (NULL, NULL);
  DBUG_ENTER ("COPYdecls");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  DECLS_DECL (result) = COPYTRAV (DECLS_DECL (arg_node), arg_info);
  DECLS_NEXT (result) = COPYTRAV (DECLS_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYdowhile
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node DoWhile node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYdowhile (node * arg_node, info * arg_info)
{
  node *result = TBmakeDowhile (NULL, NULL);
  DBUG_ENTER ("COPYdowhile");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  DOWHILE_COND (result) = COPYTRAV (DOWHILE_COND (arg_node), arg_info);
  DOWHILE_BLOCK (result) = COPYTRAV (DOWHILE_BLOCK (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYerror
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Error node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYerror (node * arg_node, info * arg_info)
{
  node *result = TBmakeError (NULL, PH_initial, NULL);
  DBUG_ENTER ("COPYerror");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  ERROR_MESSAGE (result) = STRcpy (ERROR_MESSAGE (arg_node));
  ERROR_ANYPHASE (result) = ERROR_ANYPHASE (arg_node);
  /* Copy sons */
  ERROR_NEXT (result) = COPYTRAV (ERROR_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYexprstmt
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node ExprStmt node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYexprstmt (node * arg_node, info * arg_info)
{
  node *result = TBmakeExprstmt (NULL);
  DBUG_ENTER ("COPYexprstmt");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  EXPRSTMT_EXPR (result) = COPYTRAV (EXPRSTMT_EXPR (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYexprs
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Exprs node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYexprs (node * arg_node, info * arg_info)
{
  node *result = TBmakeExprs (NULL, NULL);
  DBUG_ENTER ("COPYexprs");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  EXPRS_EXPR (result) = COPYTRAV (EXPRS_EXPR (arg_node), arg_info);
  EXPRS_NEXT (result) = COPYTRAV (EXPRS_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYfloat
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYfloat (node * arg_node, info * arg_info)
{
  node *result = TBmakeFloat (0.0);
  DBUG_ENTER ("COPYfloat");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  FLOAT_VALUE (result) = FLOAT_VALUE (arg_node);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYfor
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node For node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYfor (node * arg_node, info * arg_info)
{
  node *result = TBmakeFor (NULL, NULL, NULL, NULL, NULL);
  DBUG_ENTER ("COPYfor");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  FOR_LOOPVAR (result) = STRcpy (FOR_LOOPVAR (arg_node));
  /* Copy sons */
  FOR_START (result) = COPYTRAV (FOR_START (arg_node), arg_info);
  FOR_STOP (result) = COPYTRAV (FOR_STOP (arg_node), arg_info);
  FOR_STEP (result) = COPYTRAV (FOR_STEP (arg_node), arg_info);
  FOR_BLOCK (result) = COPYTRAV (FOR_BLOCK (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYfunbody
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node FunBody node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYfunbody (node * arg_node, info * arg_info)
{
  node *result = TBmakeFunbody (NULL, NULL, NULL);
  DBUG_ENTER ("COPYfunbody");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  FUNBODY_VARDECLS (result) =
    COPYTRAV (FUNBODY_VARDECLS (arg_node), arg_info);
  FUNBODY_LOCALFUNDEFS (result) =
    COPYTRAV (FUNBODY_LOCALFUNDEFS (arg_node), arg_info);
  FUNBODY_STMTS (result) = COPYTRAV (FUNBODY_STMTS (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYfuncall
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node FunCall node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYfuncall (node * arg_node, info * arg_info)
{
  node *result = TBmakeFuncall (NULL, NULL, NULL);
  DBUG_ENTER ("COPYfuncall");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  FUNCALL_NAME (result) = STRcpy (FUNCALL_NAME (arg_node));
  FUNCALL_DECL (result) =
    LUTsearchInLutPp (INFO_LUT (arg_info), FUNCALL_DECL (arg_node));
  /* Copy sons */
  FUNCALL_ARGS (result) = COPYTRAV (FUNCALL_ARGS (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYfundef
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node FunDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYfundef (node * arg_node, info * arg_info)
{
  node *result = TBmakeFundef (T_unknown, NULL, NULL, NULL);
  DBUG_ENTER ("COPYfundef");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  FUNDEF_TYPE (result) = FUNDEF_TYPE (arg_node);
  FUNDEF_NAME (result) = STRcpy (FUNDEF_NAME (arg_node));
  /* Copy sons */
  FUNDEF_FUNBODY (result) = COPYTRAV (FUNDEF_FUNBODY (arg_node), arg_info);
  FUNDEF_PARAMS (result) = COPYTRAV (FUNDEF_PARAMS (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYfundefs
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node FunDefs node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYfundefs (node * arg_node, info * arg_info)
{
  node *result = TBmakeFundefs (NULL, NULL);
  DBUG_ENTER ("COPYfundefs");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  FUNDEFS_FUNDEF (result) = COPYTRAV (FUNDEFS_FUNDEF (arg_node), arg_info);
  FUNDEFS_NEXT (result) = COPYTRAV (FUNDEFS_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYglobdecl
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node GlobDecl node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYglobdecl (node * arg_node, info * arg_info)
{
  node *result = TBmakeGlobdecl (T_unknown, NULL, NULL);
  DBUG_ENTER ("COPYglobdecl");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  GLOBDECL_TYPE (result) = GLOBDECL_TYPE (arg_node);
  GLOBDECL_NAME (result) = STRcpy (GLOBDECL_NAME (arg_node));
  /* Copy sons */
  GLOBDECL_DIMS (result) = COPYTRAV (GLOBDECL_DIMS (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYglobdef
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node GlobDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYglobdef (node * arg_node, info * arg_info)
{
  node *result = TBmakeGlobdef (T_unknown, NULL, NULL, NULL);
  DBUG_ENTER ("COPYglobdef");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  GLOBDEF_TYPE (result) = GLOBDEF_TYPE (arg_node);
  GLOBDEF_NAME (result) = STRcpy (GLOBDEF_NAME (arg_node));
  /* Copy sons */
  GLOBDEF_DIMS (result) = COPYTRAV (GLOBDEF_DIMS (arg_node), arg_info);
  GLOBDEF_INIT (result) = COPYTRAV (GLOBDEF_INIT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYids
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Ids node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYids (node * arg_node, info * arg_info)
{
  node *result = TBmakeIds (NULL, NULL);
  DBUG_ENTER ("COPYids");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  IDS_NAME (result) = STRcpy (IDS_NAME (arg_node));
  /* Copy sons */
  IDS_NEXT (result) = COPYTRAV (IDS_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYifelse
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node IfElse node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYifelse (node * arg_node, info * arg_info)
{
  node *result = TBmakeIfelse (NULL, NULL, NULL);
  DBUG_ENTER ("COPYifelse");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  IFELSE_COND (result) = COPYTRAV (IFELSE_COND (arg_node), arg_info);
  IFELSE_THEN (result) = COPYTRAV (IFELSE_THEN (arg_node), arg_info);
  IFELSE_ELSE (result) = COPYTRAV (IFELSE_ELSE (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYmonop
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node MonOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYmonop (node * arg_node, info * arg_info)
{
  node *result = TBmakeMonop (MO_unknown, NULL);
  DBUG_ENTER ("COPYmonop");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  MONOP_OP (result) = MONOP_OP (arg_node);
  /* Copy sons */
  MONOP_OPERAND (result) = COPYTRAV (MONOP_OPERAND (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYnum
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Num node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYnum (node * arg_node, info * arg_info)
{
  node *result = TBmakeNum (0);
  DBUG_ENTER ("COPYnum");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  NUM_VALUE (result) = NUM_VALUE (arg_node);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYparam
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Param node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYparam (node * arg_node, info * arg_info)
{
  node *result = TBmakeParam (NULL, T_unknown, NULL, NULL);
  DBUG_ENTER ("COPYparam");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  PARAM_NAME (result) = STRcpy (PARAM_NAME (arg_node));
  PARAM_TYPE (result) = PARAM_TYPE (arg_node);
  /* Copy sons */
  PARAM_DIMS (result) = COPYTRAV (PARAM_DIMS (arg_node), arg_info);
  PARAM_NEXT (result) = COPYTRAV (PARAM_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYprogram
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Program node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYprogram (node * arg_node, info * arg_info)
{
  node *result = TBmakeProgram (NULL);
  DBUG_ENTER ("COPYprogram");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  PROGRAM_SYMBOLTABLE (result) =
    LUTsearchInLutPp (INFO_LUT (arg_info), PROGRAM_SYMBOLTABLE (arg_node));
  /* Copy sons */
  PROGRAM_DECLS (result) = COPYTRAV (PROGRAM_DECLS (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYreturn
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Return node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYreturn (node * arg_node, info * arg_info)
{
  node *result = TBmakeReturn (NULL);
  DBUG_ENTER ("COPYreturn");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  RETURN_EXPR (result) = COPYTRAV (RETURN_EXPR (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYstmts
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Stmts node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYstmts (node * arg_node, info * arg_info)
{
  node *result = TBmakeStmts (NULL, NULL);
  DBUG_ENTER ("COPYstmts");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  STMTS_STMT (result) = COPYTRAV (STMTS_STMT (arg_node), arg_info);
  STMTS_NEXT (result) = COPYTRAV (STMTS_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYsymboltable
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node SymbolTable node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYsymboltable (node * arg_node, info * arg_info)
{
  node *result = TBmakeSymboltable (NULL);
  DBUG_ENTER ("COPYsymboltable");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  SYMBOLTABLE_PARENT (result) =
    LUTsearchInLutPp (INFO_LUT (arg_info), SYMBOLTABLE_PARENT (arg_node));
  SYMBOLTABLE_RETURNTYPE (result) = SYMBOLTABLE_RETURNTYPE (arg_node);
  /* Copy sons */
  SYMBOLTABLE_ENTRY (result) =
    COPYTRAV (SYMBOLTABLE_ENTRY (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYsymboltableentry
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node SymbolTableEntry node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYsymboltableentry (node * arg_node, info * arg_info)
{
  node *result = TBmakeSymboltableentry (NULL, T_unknown, 0, 0, NULL, NULL);
  DBUG_ENTER ("COPYsymboltableentry");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  SYMBOLTABLEENTRY_NAME (result) = STRcpy (SYMBOLTABLEENTRY_NAME (arg_node));
  SYMBOLTABLEENTRY_TYPE (result) = SYMBOLTABLEENTRY_TYPE (arg_node);
  SYMBOLTABLEENTRY_OFFSET (result) = SYMBOLTABLEENTRY_OFFSET (arg_node);
  SYMBOLTABLEENTRY_DEPTH (result) = SYMBOLTABLEENTRY_DEPTH (arg_node);
  SYMBOLTABLEENTRY_PARAM (result) = SYMBOLTABLEENTRY_PARAM (arg_node);
  /* Copy sons */
  SYMBOLTABLEENTRY_NEXT (result) =
    COPYTRAV (SYMBOLTABLEENTRY_NEXT (arg_node), arg_info);
  SYMBOLTABLEENTRY_TABLE (result) =
    COPYTRAV (SYMBOLTABLEENTRY_TABLE (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYternary
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Ternary node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYternary (node * arg_node, info * arg_info)
{
  node *result = TBmakeTernary (NULL, NULL, NULL);
  DBUG_ENTER ("COPYternary");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  TERNARY_EXPR (result) = COPYTRAV (TERNARY_EXPR (arg_node), arg_info);
  TERNARY_TRUE (result) = COPYTRAV (TERNARY_TRUE (arg_node), arg_info);
  TERNARY_FALSE (result) = COPYTRAV (TERNARY_FALSE (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYvar
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node Var node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYvar (node * arg_node, info * arg_info)
{
  node *result = TBmakeVar (NULL, NULL, NULL);
  DBUG_ENTER ("COPYvar");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  VAR_NAME (result) = STRcpy (VAR_NAME (arg_node));
  VAR_DECL (result) =
    LUTsearchInLutPp (INFO_LUT (arg_info), VAR_DECL (arg_node));
  /* Copy sons */
  VAR_INDICES (result) = COPYTRAV (VAR_INDICES (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYvardecl
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node VarDecl node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYvardecl (node * arg_node, info * arg_info)
{
  node *result = TBmakeVardecl (NULL, T_unknown, NULL, NULL, NULL);
  DBUG_ENTER ("COPYvardecl");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  VARDECL_NAME (result) = STRcpy (VARDECL_NAME (arg_node));
  VARDECL_TYPE (result) = VARDECL_TYPE (arg_node);
  /* Copy sons */
  VARDECL_DIMS (result) = COPYTRAV (VARDECL_DIMS (arg_node), arg_info);
  VARDECL_INIT (result) = COPYTRAV (VARDECL_INIT (arg_node), arg_info);
  VARDECL_NEXT (result) = COPYTRAV (VARDECL_NEXT (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYvarlet
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node VarLet node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYvarlet (node * arg_node, info * arg_info)
{
  node *result = TBmakeVarlet (NULL, NULL, NULL);
  DBUG_ENTER ("COPYvarlet");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy attributes */
  VARLET_NAME (result) = STRcpy (VARLET_NAME (arg_node));
  VARLET_DECL (result) =
    LUTsearchInLutPp (INFO_LUT (arg_info), VARLET_DECL (arg_node));
  /* Copy sons */
  VARLET_INDICES (result) = COPYTRAV (VARLET_INDICES (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/** <!--******************************************************************-->
 *
 * @fn COPYwhile
 *
 * @brief Copies the node and its sons/attributes
 *
 * @param arg_node While node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
COPYwhile (node * arg_node, info * arg_info)
{
  node *result = TBmakeWhile (NULL, NULL);
  DBUG_ENTER ("COPYwhile");
  LUTinsertIntoLutP (INFO_LUT (arg_info), arg_node, result);
  /* Copy sons */
  WHILE_COND (result) = COPYTRAV (WHILE_COND (arg_node), arg_info);
  WHILE_BLOCK (result) = COPYTRAV (WHILE_BLOCK (arg_node), arg_info);
  /* Return value */
  DBUG_RETURN (result);
}

/**
 * @}
 */
