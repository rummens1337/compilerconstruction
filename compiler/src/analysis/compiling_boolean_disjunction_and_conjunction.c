/*****************************************************************************
 *
 * Module: compiling boolean disjunction and conjunction
 *
 * Prefix: CBDC
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/

#include "compiling_boolean_disjunction_and_conjunction.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "free.h"
#include "copy.h"

node *CBDCbinop ( node *arg_node, info *arg_info)
{
    DBUG_ENTER("CBDCbinop");
    DBUG_PRINT ("CBDC", ("CBDCbinop"));

    // only intrested in logic operations
    if (BO_or != BINOP_OP ( arg_node) && BO_and != BINOP_OP ( arg_node)) DBUG_RETURN( arg_node);
    
    // placeholder for the left node
    node *left = NULL;

    // traverse over the right node
    BINOP_LEFT( arg_node) = TRAVopt( BINOP_LEFT( arg_node), arg_info);
    BINOP_RIGHT( arg_node) = TRAVopt( BINOP_RIGHT( arg_node), arg_info);

    // define the left expression
    if (BO_or == BINOP_OP ( arg_node))
    {
        // create the right expression
        node *right = BINOP_RIGHT( arg_node) == NULL ? TBmakeBool(0) : TBmakeTernary (COPYdoCopy(BINOP_RIGHT( arg_node)), TBmakeBool(1), TBmakeBool(0));

        // define the left node
        left = TBmakeTernary (COPYdoCopy(BINOP_LEFT( arg_node)), TBmakeBool(1), right);
    }
    else if (BO_and == BINOP_OP ( arg_node))
    {
        // create the right expression
        node *right = BINOP_RIGHT( arg_node) == NULL ? TBmakeBool(1) : TBmakeTernary (COPYdoCopy(BINOP_RIGHT( arg_node)), TBmakeBool(1), TBmakeBool(0));

        // define the left node
        left = TBmakeTernary (COPYdoCopy(BINOP_LEFT( arg_node)), right, TBmakeBool(0));
    }

    // free the node
    FREEdoFreeTree(arg_node);

    // assign the ternary expression
    arg_node = left;

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */
node *CBDCdoCompilingBooleanDisjunctionAndConjunction(node *syntaxtree)
{
    DBUG_ENTER("CBDCdoCompilingBooleanDisjunctionAndConjunction");
    DBUG_PRINT ("CBDC", ("CBDCdoCompilingBooleanDisjunctionAndConjunction"));

    TRAVpush( TR_cbdc);
    syntaxtree = TRAVdo(syntaxtree, NULL);
    TRAVpop();

    DBUG_RETURN(syntaxtree);
}
