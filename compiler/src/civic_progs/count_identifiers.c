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
#include "../framework/lookup_table.h"


/*
 * INFO structure
 */

struct INFO {
    lut_t * lut;
};

struct COUNTER {
    int count;
    char * identifier;
};

typedef struct COUNTER counter;

/*
 * INFO macros
 */

#define INFO_LUT(n)  ((n)->lut)
#define COUNTER_COUNT(n) ((n)->count)
#define COUNTER_IDENTIFIER(n) ((n)->identifier)

/*
 * INFO functions
 */

static info *MakeInfo(void)
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));

    INFO_LUT(result) = LUTgenerateLut();

    DBUG_RETURN(result);
}


static info *FreeInfo(info *info) {
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

static void * print(void *item)
{
    counter * _counter = (counter *)item;
    CTInote( "%s = %d", _counter->identifier, _counter->count);
    return item;
}

static counter *MakeCounter(void)
{
    counter *result;

    DBUG_ENTER("MakeCounter");

    result = (counter *)MEMmalloc(sizeof(counter));

    COUNTER_COUNT(result) = 1;
    COUNTER_IDENTIFIER(result) = NULL;

    DBUG_RETURN(result);
}

static void insertIntoLut(info *arg_info, char *identifier)
{
    lut_t *lut = INFO_LUT(arg_info);
    void **result = LUTsearchInLutS(lut, identifier);
    counter * _counter;

    if(result == NULL)
    {
        _counter = MakeCounter();
        COUNTER_IDENTIFIER(_counter) = STRcpy(identifier);

        lut = LUTinsertIntoLutS(lut, identifier, _counter);
    }
    else
    {
        void *found_item;
        _counter = (counter *)(*result);
        COUNTER_COUNT(_counter) += 1;

        lut = LUTupdateLutS(lut, identifier, _counter, &found_item);
    }

    CTInote("Identifier '%s' occurs: %d", _counter->identifier, _counter->count);
}

/*
 * Traversal functions
 */

node *CIvar(node *arg_node, info *arg_info) {
    DBUG_ENTER("CIvar");

    char *identifier = VAR_NAME(arg_node);

    CTInote( "Expression: %s", identifier);
    insertIntoLut(arg_info, identifier);

    DBUG_RETURN(arg_node);
}

node *CIvarlet(node *arg_node, info *arg_info) {
    DBUG_ENTER("CImodule");

    char *identifier = VARLET_NAME(arg_node);

    CTInote( "Assignment: %s", identifier);
    insertIntoLut(arg_info, identifier);

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
    syntaxtree = TRAVdo(syntaxtree, arg_info);

    lut_t *lut = INFO_LUT(arg_info);

    LUTmapLutS(lut, print);

    TRAVpop();

    DBUG_RETURN(syntaxtree);

    arg_info = FreeInfo(arg_info);
}
