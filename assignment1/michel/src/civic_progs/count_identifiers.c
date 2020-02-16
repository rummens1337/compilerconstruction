/*****************************************************************************
 *
 * Module: count_identifiers
 *
 * Prefix: CI
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
#include "lookup_table.h"


/*
 * INFO structure
 */

struct INFO {
    lut_t * lut;
    int counter;
};

/*
 * INFO macros
 */

#define INFO_LUT(n)  ((n)->lut)
#define INFO_COUNTER(n)  ((n)->counter)

/*
 * INFO functions
 */

static info *MakeInfo(void)
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));

    INFO_LUT(result) = LUTgenerateLut();
    INFO_COUNTER(result);

    DBUG_RETURN(result);
}


static info *FreeInfo(info *info) {
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

static void insertIntoLut(char *identifier)
{

    info *info = MakeInfo();
    lut_t *lut = INFO_LUT(info);
    int *counter = INFO_COUNTER(info);

    lut = LUTinsertIntoLutS(lut, identifier, counter);
    INFO_COUNTER(info) += 1;

    CTInote("Counter value: %d", INFO_COUNTER(info));
}

/*
 * Traversal functions
 */

node *CIvar(node *arg_node, info *arg_info) {
    DBUG_ENTER("CIvar");
    info *info = MakeInfo();

    char *identifier = VAR_NAME(arg_node);
    insertIntoLut(identifier);

    CTInote( "Expression: %s", identifier);

    DBUG_RETURN(arg_node);
}

node *CIvarlet(node *arg_node, info *arg_info) {
    DBUG_ENTER("CImodule");
    info *info = MakeInfo();

    char *identifier = VARLET_NAME(arg_node);
    insertIntoLut(identifier);

    CTInote( "Assignment: %s", identifier);

    DBUG_RETURN(arg_node);
}

/*
 * Traversal start function
 */

node *CIdoCountIdentifiers(node *syntaxtree) {
    info *arg_info;

    DBUG_ENTER("CIdoCountIdentifiers");
    arg_info = MakeInfo();

    TRAVpush(TR_ci);
    syntaxtree = TRAVdo(syntaxtree, NULL);

    TRAVpop();

    DBUG_RETURN(syntaxtree);

    arg_info = FreeInfo(arg_info);
}
