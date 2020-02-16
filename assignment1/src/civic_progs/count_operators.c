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

    if (BINOP_OP( arg_node) == BO_add) MODULE_ADD( arg_node) += 1;
    if (BINOP_OP( arg_node) == BO_add) MODULE_SUB( arg_node) += 1;
    if (BINOP_OP( arg_node) == BO_add) MODULE_DIV( arg_node) += 1;
    if (BINOP_OP( arg_node) == BO_add) MODULE_MUL( arg_node) += 1;
    if (BINOP_OP( arg_node) == BO_add) MODULE_MOD( arg_node) += 1;

//    if (BINOP_OP( arg_node) == BO_sub) {
//        if ((NODE_TYPE( BINOP_LEFT( arg_node)) == N_var)
//            && (NODE_TYPE( BINOP_RIGHT( arg_node)) == N_var)
//            && STReq( VAR_NAME( BINOP_LEFT( arg_node)), VAR_NAME( BINOP_RIGHT( arg_node)))) {
//            arg_node = FREEdoFreeTree( arg_node);
//            arg_node = TBmakeNum( 0);
//        }
//        else if  ((NODE_TYPE( BINOP_LEFT( arg_node)) == N_num)
//                  && (NODE_TYPE( BINOP_RIGHT( arg_node)) == N_num)
//                  && (NUM_VALUE( BINOP_LEFT( arg_node)) == NUM_VALUE( BINOP_RIGHT( arg_node)))) {
//            arg_node = FREEdoFreeTree( arg_node);
//            arg_node = TBmakeNum( 0);
//        }
//    }

    DBUG_RETURN( arg_node);
}

node *COmodule (node *arg_node, info *arg_info)
{
    DBUG_ENTER("CObinop");

    /*
     * Extremely important:
     *  we must continue to traverse the abstract syntax tree !!
     */
    BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);
    BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);

    if (BINOP_OP( arg_node) == BO_sub) {
        if ((NODE_TYPE( BINOP_LEFT( arg_node)) == N_var)
            && (NODE_TYPE( BINOP_RIGHT( arg_node)) == N_var)
            && STReq( VAR_NAME( BINOP_LEFT( arg_node)), VAR_NAME( BINOP_RIGHT( arg_node)))) {
            arg_node = FREEdoFreeTree( arg_node);
            arg_node = TBmakeNum( 0);
        }
        else if  ((NODE_TYPE( BINOP_LEFT( arg_node)) == N_num)
                  && (NODE_TYPE( BINOP_RIGHT( arg_node)) == N_num)
                  && (NUM_VALUE( BINOP_LEFT( arg_node)) == NUM_VALUE( BINOP_RIGHT( arg_node)))) {
            arg_node = FREEdoFreeTree( arg_node);
            arg_node = TBmakeNum( 0);
        }
    }

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */

node *COdoCountOperators( node *syntaxtree)
{
    DBUG_ENTER("COdoCountOperators");

    TRAVpush( TR_co);
    syntaxtree = TRAVdo( syntaxtree, NULL);
    TRAVpop();

    DBUG_RETURN( syntaxtree);
}
