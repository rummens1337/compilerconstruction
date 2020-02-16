/*****************************************************************************
 *
 * Module: count_operators
 *
 * Prefix: CO
 *
 * Description:
 *
 * This module implements a demo traversal of the abstract syntax tree that
 * counts all operators and prints the result at the end of the traversal.
 *
 *****************************************************************************/


#include "opt_sub.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
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

node *CObinop (node *arg_node, info *arg_info)
{
    DBUG_ENTER("CObinop");

    /*
     * Extremely important:
     *  we must continue to traverse the abstract syntax tree !!
     */
    BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);
    BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);

    if (BINOP_OP( arg_node) == BO_add) INFO_ADD( arg_info) += 1;
    if (BINOP_OP( arg_node) == BO_add) INFO_SUB( arg_info) += 1;
    if (BINOP_OP( arg_node) == BO_add) INFO_MUL( arg_info) += 1;
    if (BINOP_OP( arg_node) == BO_add) INFO_DIV( arg_info) += 1;
    if (BINOP_OP( arg_node) == BO_add) INFO_MOD( arg_info) += 1;

    DBUG_RETURN( arg_node);
}

node *COmodule (node *arg_node, info *arg_info)
{
    DBUG_ENTER("COmodule");

    info * info = MakeInfo();

    // traverse over the statement nods
    TRAVdo(MODULE_NEXT(arg_node), info);

    MODULE_ADD(arg_node) = INFO_ADD(info);
    MODULE_SUB(arg_node) = INFO_SUB(info);
    MODULE_MUL(arg_node) = INFO_MUL(info);
    MODULE_DIV(arg_node) = INFO_DIV(info);
    MODULE_MOD(arg_node) = INFO_MOD(info);

    DBUG_RETURN(arg_node);
}

node *COstmts(node *arg_node, info *arg_info)
{
    DBUG_ENTER("COstmts");

    /*
     * Extremely important:
     *  we must continue to traverse the abstract syntax tree !!
     */

    STMTS_STMT(arg_node) = TRAVdo(STMTS_STMT(arg_node), arg_info);
    STMTS_NEXT(arg_node) = TRAVopt(STMTS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */

node *COdoCountOperators( node *syntaxtree)
{
    info *arg_info;

    DBUG_ENTER("COdoCountOperators");
    arg_info = MakeInfo();

    TRAVpush( TR_co);
    syntaxtree = TRAVdo( syntaxtree, NULL);
    TRAVpop();

    // Not sure why this works in sum_ints.c but not here :/
    // todo: find out why this works in sum_ints.c
    CTInote( "Sum of additions: %d", INFO_ADD( arg_info));
    CTInote( "Sum of subtractions: %d", INFO_SUB( arg_info));
    CTInote( "Sum of divisions: %d", INFO_DIV( arg_info));
    CTInote( "Sum of multiplications: %d", INFO_MUL( arg_info));
    CTInote( "Sum of modulo's: %d", INFO_MOD( arg_info));

    DBUG_RETURN( syntaxtree);

    arg_info = FreeInfo( arg_info);
}
