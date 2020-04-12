/*****************************************************************************
 *
 * Module: check range
 *
 * Prefix: CR
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/

#include "check_range.h"

#include <limits.h>
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
    size_t total;
    size_t _int;
    float _float;
    type expr_type;
    binop op;
};

#define INFO_SYMBOL_TABLE(n) ((n)->symbol_table)
#define INFO_TOTAL(n) ((n)->total)
#define INFO_INT(n) ((n)->_int)
#define INFO_FLOAT(n) ((n)->_float)
#define INFO_EXPR_TYPE(n) ((n)->expr_type)
#define INFO_OP(n) ((n)->op)


// integer value out of range (signed 32-bit)

/*
 * INFO functions
 */

static info *MakeInfo()
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));
    INFO_SYMBOL_TABLE ( result) = NULL;
    INFO_TOTAL(result) = 0;
    INFO_INT(result) = 0;
    INFO_FLOAT(result) = 0;
    INFO_EXPR_TYPE (result) = T_unknown;
    INFO_OP (result) = T_unknown;

    DBUG_RETURN(result);
}

static info *FreeInfo(info *info)
{
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

node *CRprogram(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CRprogram");
    DBUG_PRINT("CR", ("CRprogram"));

    // link the symbol table
    INFO_SYMBOL_TABLE(arg_info) = PROGRAM_SYMBOLTABLE(arg_node);

    PROGRAM_DECLS(arg_node) = TRAVdo ( PROGRAM_DECLS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *CRbinop (node * arg_node, info * arg_info)
{
    DBUG_ENTER("CRbinop");
    DBUG_PRINT ("CR", ("CRbinop"));

    switch (BINOP_OP( arg_node))
    {
        case BO_add:
        case BO_sub:
        case BO_mul:
        case BO_div:
            INFO_OP ( arg_info) = BINOP_OP (arg_node);
        break;
        case BO_lt:
        case BO_le:
        case BO_gt:
        case BO_ge:
        case BO_eq:
        case BO_ne:
        case BO_and:
        case BO_or:
        case BO_mod:
        case BO_unknown:
            break;
    }

    BINOP_LEFT ( arg_node) = TRAVopt ( BINOP_LEFT ( arg_node), arg_info);
    BINOP_RIGHT ( arg_node) = TRAVopt ( BINOP_RIGHT ( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *CRcast ( node *arg_node, info *arg_info)
{
    DBUG_ENTER("CRcast");
    DBUG_PRINT ("CR", ("CRcast"));

    // traverse over the expressions
    CAST_EXPR ( arg_node) = TRAVdo ( CAST_EXPR ( arg_node), arg_info);

    // the type
    INFO_EXPR_TYPE (arg_info) = CAST_TYPE ( arg_node);


    DBUG_RETURN( arg_node);
}

node *CRvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("CRvar");
    DBUG_PRINT ("CR", ("CRvar"));

    // get the entry
    node *node = STdeepSearchVariableByName( INFO_SYMBOL_TABLE ( arg_info), VAR_NAME( arg_node));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = SYMBOLTABLEENTRY_TYPE ( node);

    DBUG_RETURN( arg_node);
}

node *CRnum (node * arg_node, info * arg_info)
{
    DBUG_ENTER("CRnum");
    DBUG_PRINT ("CR", ("CRnum"));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = T_int;

    DBUG_RETURN( arg_node);
}

node *CRfloat (node * arg_node, info * arg_info)
{
    DBUG_ENTER("CRfloat");
    DBUG_PRINT ("CR", ("CRfloat"));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = T_float;

    DBUG_RETURN( arg_node);
}

node *CRbool (node * arg_node, info * arg_info)
{
    DBUG_ENTER("CRbool");
    DBUG_PRINT ("CR", ("CRbool"));

    // set the type
    INFO_EXPR_TYPE ( arg_info) = T_bool;

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */
node *CRdoCheckRange(node *syntaxtree)
{
    DBUG_ENTER("CRdoCheckRange");
    DBUG_PRINT ("CR", ("CRdoCheckRange"));

    info *info = MakeInfo();

    TRAVpush( TR_cr);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
