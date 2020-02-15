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
 * Traversal functions
 */

node *CAObinop(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CAObinop");

    CTInote("CAObinop");
    CTInote("TYPE: %d", NODE_TYPE(arg_node));

    BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);
    BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);


    // @todo check the operator and somehow pass it to the parent or something
    //
    // if (BINOP_OP( arg_node) == BO_add) MODULE_ADD( arg_node) += 1;
    // if (BINOP_OP( arg_node) == BO_sub) MODULE_SUB( arg_node) += 1;
    // if (BINOP_OP( arg_node) == BO_mul) MODULE_MUL( arg_node) += 1;
    // if (BINOP_OP( arg_node) == BO_div) MODULE_DIV( arg_node) += 1;
    // if (BINOP_OP( arg_node) == BO_mod) MODULE_MOD( arg_node) += 1;

    DBUG_RETURN(arg_node);
}

node *CAOstmts(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CAOstmts");

    CTInote("CAOstmts");
    CTInote("TYPE: %d", NODE_TYPE(arg_node));
    // CTInote( "LINE: %d", NODE_LINE(arg_node));
    // CTInote( "COL: %d", NODE_COL(arg_node));
    // CTInote("SONS: %d", TRAVnumSons(arg_node));

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
    DBUG_ENTER("CAOmodule");

    CTInote("CAOmodule");
    CTInote("Type: %d", NODE_TYPE(arg_node));
    CTInote("SONS: %d", TRAVnumSons(arg_node));

    // traverse over the statement nods
    TRAVdo(MODULE_NEXT(arg_node), arg_info);


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
