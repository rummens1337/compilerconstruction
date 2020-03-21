/*****************************************************************************
 *
 * Module: type_checker
 *
 * Prefix: sa
 *
 * Description:
 *
 * This module implements a demo traversal of the abstract syntax tree that
 * checks all used types and prints errors if there are any at the end 
 * of the traversal.
 *
 *****************************************************************************/

#include "type_checker.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "memory.h"
#include "free.h"
#include "str.h"
#include "ctinfo.h"
#include "../framework/lookup_table.h"

struct INFO
{
    node *symtab;
};

#define INFO_SYMTAB(n) ((n)->symtab)

static info *MakeInfo(void)
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(result));

    DBUG_RETURN(result);
}

/*
 * Traversal functions
 */

node *TCassign(node *arg_node, info *arg_info)
{   
    DBUG_ENTER("TCassign");

    CTInote("Sum of assign: %d", INFO_ADD(arg_info));

    DBUG_RETURN(arg_node);
}

node *TCifelse(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TCifelse");

    DBUG_RETURN(arg_node);
}

node *TCwhile(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TCwhile");

    DBUG_RETURN(arg_node);
}

node *TCdowhile(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TCdowhile");

    DBUG_RETURN(arg_node);
}

node *TCfor(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TCfor");

    DBUG_RETURN(arg_node);
}

/*
 * Traversal start function
 */

node *TCdoTypeChecker(node *syntaxtree)
{
    info *arg_info;
    arg_info = MakeInfo();

    DBUG_ENTER("TCdoTypeChecker");

    TRAVpush(TR_tc);
    syntaxtree = TRAVdo(syntaxtree, arg_info);
    TRAVpop();
    
    DBUG_RETURN(syntaxtree);
}
