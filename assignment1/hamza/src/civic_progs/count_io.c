/******************************************************************************
 *
 * Module: count_io
 *
 * Prefix: CIA
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/

#include "count_io.h"

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
#include "lookup_table.h"

/*
 * INFO structure
 */

struct INFO
{
    lut_t *lut;
};

/*
 * INFO macros
 */

#define INFO_LUT(n) ((n)->lut)

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

static info *FreeInfo(info *info)
{
    DBUG_ENTER("FreeInfo");

    LUTremoveContentLut(INFO_LUT(info));
    LUTremoveLut(INFO_LUT(info));
    info = MEMfree(info);

    DBUG_RETURN(info);
}

/*
 * INFO structure
 */

struct COUNTER
{
    int sum;
};

typedef struct COUNTER counter;

/*
 * INFO macros
 */

#define COUNTER_SUM(n) ((n)->sum)

/*
 * INFO functions
 */

static counter *MakeCounter(void)
{
    DBUG_ENTER("MakeCounter");

    counter* result = (counter *)MEMmalloc(sizeof(counter));

    COUNTER_SUM(result) = 0;

    DBUG_RETURN(result);
}

static counter *FreeCounter(counter *info)
{
    DBUG_ENTER("FreeCounter");

    info = MEMfree(info);

    DBUG_RETURN(info);
}


static void *_map(void *item)
{
    CTInote( "CI %s", item);
    return item;
}


static void process(info *arg_info, char *name)
{
    lut_t *lut = INFO_LUT(arg_info);
    counter *_counter = (counter *)LUTsearchInLutS(lut, name);

    if (_counter == NULL)
    {
        CTInote( "1. found %s", name);

        _counter = MakeCounter();
        COUNTER_SUM(_counter) = 1;

        CTInote( "2. found: %s = %d", name, COUNTER_SUM(_counter));

        lut = LUTinsertIntoLutS(lut, name, _counter);

        _counter = (counter *)LUTsearchInLutS(lut, name);

        CTInote( "3. found: %s = %d", name, _counter->sum);

    }
    else
    {
        void *found;

        CTInote( "1. process: %s = %d", name, COUNTER_SUM(_counter));

        COUNTER_SUM(_counter) += 1;

        CTInote( "2. process: %s = %d", name, COUNTER_SUM(_counter));

        //
        lut = LUTupdateLutS(lut, name, _counter, &found);
    }
}
/*
 * Traversal functions
 */

node *CIOmodule(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CAOstmts");

    CTInote( "CIOmodule");

    info *info = MakeInfo();

    // traverse over the nodes
    MODULE_NEXT(arg_node) = TRAVdo(MODULE_NEXT(arg_node), info);

    LUTmapLutS(INFO_LUT(info), _map);

    info = FreeInfo(info);


    DBUG_RETURN(arg_node);
}

node *CIOvarlet(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CIOvarlet");

    char *name = VARLET_NAME(arg_node);

    CTInote( "CIOvarlet: %s", name);

    DBUG_PRINT( "CIO", ("CIOvarlet: Name = %s", name));

    process(arg_info, name);

    DBUG_RETURN(arg_node);
}

node *CIOvar(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CIOvar");

    char *name = VAR_NAME(arg_node);

    CTInote( "CIOvar: %s", name);

    DBUG_PRINT( "CIO", ("CIOvar: Name = %s", name));

    process(arg_info, name);


    DBUG_RETURN(arg_node);
}

/*
 * Traversal start function
 */

node *CIOdoCountAo(node *syntaxtree)
{
    DBUG_ENTER("CIOdoCountAo");

    // push the cio sub phase to the stack
    TRAVpush(TR_cio);

    // traverse over all the nodes
    syntaxtree = TRAVdo(syntaxtree, NULL);

    // pop it from the stack
    TRAVpop();

    // result would ow be available if the arg_info argument was provided

    DBUG_RETURN(syntaxtree);
}
