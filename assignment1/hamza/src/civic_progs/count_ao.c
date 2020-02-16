/******************************************************************************
 *
 * Module: count_ao
 *
 * Prefix: CAO
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/

#include "count_ao.h"

#include "print.h"
#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "traverse_helper.h"
#include "dbug.h"

#include "memory.h"
#include "free.h"
#include "str.h"
#include "ctinfo.h"

/*
 * INFO structure
 */

struct INFO {
  int add;
  int sub;
  int mul;
  int div;
  int mod;
};

/*
 * INFO macros
 */

#define INFO_ADD(n)  ((n)->add)
#define INFO_SUB(n)  ((n)->sub)
#define INFO_MUL(n)  ((n)->mul)
#define INFO_DIV(n)  ((n)->div)
#define INFO_MOD(n)  ((n)->mod)

/*
 * INFO functions
 */

static info *MakeInfo(void)
{
  info *result;

  DBUG_ENTER( "MakeInfo");

  result = (info *)MEMmalloc(sizeof(info));

  INFO_ADD( result) = 0;
  INFO_SUB( result) = 0;
  INFO_MUL( result) = 0;
  INFO_DIV( result) = 0;
  INFO_MOD( result) = 0;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
  DBUG_ENTER ("FreeInfo");

  info = MEMfree( info);

  DBUG_RETURN( info);
}
/*
 * Traversal functions
 */

node *CAObinop(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CAObinop");

    // travsere further in the tree
    BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);
    BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);

    // increment the ops
    if (BINOP_OP( arg_node) == BO_add) INFO_ADD(arg_info) += 1;
    else if (BINOP_OP( arg_node) == BO_sub) INFO_SUB(arg_info) += 1;
    else if (BINOP_OP( arg_node) == BO_mul) INFO_MUL(arg_info) += 1;
    else if (BINOP_OP( arg_node) == BO_div) INFO_DIV(arg_info) += 1;
    else if (BINOP_OP( arg_node) == BO_mod) INFO_MOD(arg_info) += 1;

    DBUG_RETURN(arg_node);
}

node *CAOstmts(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CAOstmts");

    // traverse over the nodes
    STMTS_STMT(arg_node) = TRAVdo(STMTS_STMT(arg_node), arg_info);
    STMTS_NEXT(arg_node) = TRAVopt(STMTS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

/*
 * Traversal functions
 */

node *CAOmodule(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CAOmodule begin");

    info * info = MakeInfo();

    // traverse over the statement nods
    TRAVdo(MODULE_NEXT(arg_node), info);

    MODULE_ADD(arg_node) = INFO_ADD(info);
    MODULE_SUB(arg_node) = INFO_SUB(info);
    MODULE_MUL(arg_node) = INFO_MUL(info);
    MODULE_DIV(arg_node) = INFO_DIV(info);
    MODULE_MOD(arg_node) = INFO_MOD(info);

    info = FreeInfo( info);

    DBUG_RETURN(arg_node);
}

/*
 * Traversal start function
 */

node *CAOdoCountAo(node *syntaxtree)
{
    DBUG_ENTER("CAOdoCountAo");

    // push the coa sub phase to the stack
    TRAVpush(TR_cao);

    // traverse over all the nodes
    syntaxtree = TRAVdo(syntaxtree, NULL);

    // pop it from the stack
    TRAVpop();

    // result would ow be available if the arg_info argument was provided

    DBUG_RETURN(syntaxtree);
}
