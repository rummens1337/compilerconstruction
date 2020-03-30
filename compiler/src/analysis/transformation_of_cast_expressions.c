/*****************************************************************************
 *
 * Module: transformation of cast expressions
 *
 * Prefix: TOCE
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/

#include "transformation_of_cast_expressions.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "free.h"
#include "copy.h"

node *TOCEcast ( node *arg_node, info *arg_info)
{
    DBUG_ENTER("TOCEbinop");
    DBUG_PRINT ("TOCE", ("TOCEbinop"));

    // only intrested in logic operations
    if (T_bool != CAST_TYPE ( arg_node)) DBUG_RETURN( arg_node);

    // assign the ternary expression
    node *ternary = TBmakeTernary(COPYdoCopy(CAST_EXPR ( arg_node)), TBmakeBool(1), TBmakeBool(0));

    // free the node
    FREEdoFreeTree(arg_node);

    // set the new node
    arg_node = ternary;

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */
node *TOCEdoTransformationOfCastExpressions(node *syntaxtree)
{
    DBUG_ENTER("TOCEdoTransformationOfCastExpressions");
    DBUG_PRINT ("TOCE", ("TOCEdoTransformationOfCastExpressions"));

    TRAVpush( TR_toce);
    syntaxtree = TRAVdo(syntaxtree, NULL);
    TRAVpop();

    DBUG_RETURN(syntaxtree);
}
