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

#include "type_checking.h"
#include "symbol_table.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"
#include "print.h"

#include "memory.h"
#include "free.h"
#include "str.h"
#include "copy.h"
#include "ctinfo.h"

/*
 * INFO structure
 */
struct INFO
{
    node *symbol_table;
    type expr_type;
};

#define INFO_SYMBOL_TABLE(n) ((n)->symbol_table)
#define INFO_EXPR_TYPE(n) ((n)->expr_type)

/*
 * INFO functions
 */

static info *MakeInfo()
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));
    INFO_SYMBOL_TABLE ( result) = NULL;
    INFO_EXPR_TYPE ( result) = T_unknown;

    DBUG_RETURN(result);
}

static info *FreeInfo(info *info)
{
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

node *TOCEprogram(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TOCEprogram");
    DBUG_PRINT("TOCE", ("TOCEprogram"));

    // link the symbol table
    INFO_SYMBOL_TABLE(arg_info) = PROGRAM_SYMBOLTABLE(arg_node);

    PROGRAM_DECLS(arg_node) = TRAVdo ( PROGRAM_DECLS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *TOCEfundef(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TOCEfundef");
    DBUG_PRINT ("TOCE", ("TOCEfundef"));

    // rember the table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    node *entry = STsearchFundef( table, FUNDEF_NAME ( arg_node));

    INFO_SYMBOL_TABLE ( arg_info) = SYMBOLTABLEENTRY_TABLE ( entry);

    // traverse over the body
    FUNDEF_FUNBODY ( arg_node) = TRAVopt ( FUNDEF_FUNBODY ( arg_node), arg_info);

    INFO_SYMBOL_TABLE ( arg_info) = table;

    DBUG_RETURN( arg_node);
}

node *TOCEbinop (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TOCEbinop");
    DBUG_PRINT ("TOCE", ("TOCEbinop"));

    BINOP_LEFT ( arg_node) = TRAVopt ( BINOP_LEFT ( arg_node), arg_info);
    BINOP_RIGHT ( arg_node) = TRAVopt ( BINOP_RIGHT ( arg_node), arg_info);

    switch (BINOP_OP( arg_node))
    {
        case BO_lt:
        case BO_le:
        case BO_gt:
        case BO_ge:

            INFO_EXPR_TYPE ( arg_info) = T_bool;

        break;

        case BO_eq:
        case BO_ne:

            // BINOP_LEFT ( arg_node) = TBmakeTernary(BINOP_LEFT ( arg_node), TBmakeNum(1), TBmakeNum(0));
            // BINOP_RIGHT ( arg_node) = TBmakeTernary(BINOP_RIGHT ( arg_node), TBmakeNum(1), TBmakeNum(0));

            INFO_EXPR_TYPE ( arg_info) = T_bool;

            break;
        case BO_add:
        case BO_sub:
        case BO_mul:
        case BO_div:
        case BO_and:
        case BO_or:
        case BO_mod:
        case BO_unknown:
            break;
    }

    DBUG_RETURN( arg_node);
}

node *TOCEcast ( node *arg_node, info *arg_info)
{
    DBUG_ENTER("TOCEcast");
    DBUG_PRINT ("TOCE", ("TOCEcast"));

    // traverse over the expressions
    CAST_EXPR ( arg_node) = TRAVdo ( CAST_EXPR ( arg_node), arg_info);

    // store the expresion
    node *expr = COPYdoCopy ( CAST_EXPR ( arg_node));

    // the type
    type casttype = CAST_TYPE ( arg_node);

    // only intrested in logic operations
    if (casttype == T_bool)
    {
        // free the node
        FREEdoFreeTree(arg_node);

        // number to compare to
        node *num = INFO_EXPR_TYPE ( arg_info) == T_int ? TBmakeNum(0) : TBmakeFloat(0.0);

        // set the new node
        arg_node = TBmakeBinop(BO_ne, expr, num);
    }
    else if (INFO_EXPR_TYPE ( arg_info) == T_bool)
    {
        // free the node
        FREEdoFreeTree(arg_node);

        // set the new node
        arg_node = TBmakeTernary(
            expr,
            casttype == T_int ? TBmakeNum(1) : TBmakeFloat(1.0),
            casttype == T_int ? TBmakeNum(0) : TBmakeFloat(0.0)
        );
    }
    else
    {
        FREEdoFreeTree(expr);
    }
    

    DBUG_RETURN( arg_node);
}

node *TOCEvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TOCEvar");
    DBUG_PRINT ("TOCE", ("TOCEvar"));

    // get the entry
    node *node = STdeepSearchVariableByName( INFO_SYMBOL_TABLE ( arg_info), VAR_NAME( arg_node));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = SYMBOLTABLEENTRY_TYPE ( node);

    DBUG_RETURN( arg_node);
}

node *TOCEnum (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TOCEnum");
    DBUG_PRINT ("TOCE", ("TOCEnum"));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = T_int;

    DBUG_RETURN( arg_node);
}

node *TOCEfloat (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TOCEfloat");
    DBUG_PRINT ("TOCE", ("TOCEfloat"));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = T_float;

    DBUG_RETURN( arg_node);
}

node *TOCEbool (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TOCEbool");
    DBUG_PRINT ("TOCE", ("TOCEbool"));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = T_bool;

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */
node *TOCEdoTransformationOfCastExpressions(node *syntaxtree)
{
    DBUG_ENTER("TOCEdoTransformationOfCastExpressions");
    DBUG_PRINT ("TOCE", ("TOCEdoTransformationOfCastExpressions"));

    info *info = MakeInfo();

    TRAVpush( TR_toce);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
