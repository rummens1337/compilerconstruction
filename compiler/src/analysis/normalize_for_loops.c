/*****************************************************************************
 *
 * Module: normalize for loops
 *
 * Prefix: NFL
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/


#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "normalize_for_loops.h"
#include "key_value_linked_list.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "memory.h"
#include "free.h"
#include "str.h"
#include "copy.h"
#include "ctinfo.h"

#include "print.h"

/*
 * INFO structure
 */

struct INFO {
  node *vardecls;
  node *stmts;
  kvlistnode *names;
};

/*
 * INFO macros
 */
#define INFO_VARDECLS(n)  ((n)->vardecls)
#define INFO_STMTS(n)  ((n)->stmts)
#define INFO_NAMES(n)  ((n)->names)


void append(node *front, node *new)
{
    // no need to continue if the node is empty
    if (front == NULL) return;

    if (NODE_TYPE ( front) == N_stmts)
    {
        // did we reach the end?
        if (STMTS_NEXT ( front) == NULL) STMTS_NEXT (front) = new;

        // get the next node
        else append(STMTS_NEXT(front), new);
    }

    else if (NODE_TYPE ( front) == N_vardecl)
    {
        // did we reach the end?
        if (VARDECL_NEXT ( front) == NULL) VARDECL_NEXT (front) = new;

        // get the next node
        else append(VARDECL_NEXT(front), new);
    }
}
/*
 * INFO functions
 */

static info *MakeInfo()
{
  info *result;

  DBUG_ENTER( "MakeInfo");

  result = (info *)MEMmalloc(sizeof(info));

  INFO_VARDECLS ( result) = NULL;
  INFO_STMTS ( result) = NULL;
  INFO_NAMES ( result) = NULL;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
    DBUG_ENTER ("FreeInfo");

    // free the list
    KVLLdispose(INFO_NAMES (info));

    info = MEMfree( info);

    DBUG_RETURN( info);
}

node *NFLfunbody(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLfunbody");
    DBUG_PRINT ("NFL", ("NFLfunbody"));

    // traverse over the sons
    FUNBODY_STMTS(arg_node) = TRAVopt ( FUNBODY_STMTS(arg_node), arg_info);

    // do we need to add vardecls?
    node *vardecls = INFO_VARDECLS ( arg_info);

    if ( vardecls == NULL) DBUG_RETURN( arg_node);

    // append the var decls
    if (FUNBODY_VARDECLS ( arg_node) == NULL) FUNBODY_VARDECLS ( arg_node) = vardecls;

    // add the decls
    else append(FUNBODY_VARDECLS ( arg_node), vardecls);

    // reset the variable
    INFO_VARDECLS ( arg_info) = NULL;

    // done
    DBUG_RETURN( arg_node);
}

node *NFLstmts(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLstmts");
    DBUG_PRINT ("NFL", ("NFLstmts"));

    nodetype type = NODE_TYPE ( STMTS_STMT ( arg_node));

    STMTS_STMT ( arg_node) = TRAVdo ( STMTS_STMT ( arg_node), arg_info);

    if (type == N_for)
    {
        node *oldnode = arg_node;
        append(INFO_STMTS (arg_info), arg_node);
        arg_node = INFO_STMTS (arg_info);
        INFO_STMTS (arg_info) = NULL;

        STMTS_NEXT ( oldnode) = TRAVopt ( STMTS_NEXT ( oldnode), arg_info);
    }
    else
    {
        STMTS_NEXT ( arg_node) = TRAVopt ( STMTS_NEXT ( arg_node), arg_info);
    }

    // done
    DBUG_RETURN( arg_node);
}
node *NFLfor(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLfor");
    DBUG_PRINT ("NFL", ("NFLfor"));

    // remember the var name
    const char *varname = FOR_LOOPVAR (arg_node);

    // generate random number
    int index = rand(); 

    char *cindex = STRitoa(index);

    // set the new name
    char *name = STRcatn ( 4, "__for_", cindex, "_" , varname);

    // free the string
    free(cindex);

    // add the name to the list
    if (INFO_NAMES( arg_info) == NULL) INFO_NAMES( arg_info) = KVLLcreate(varname, name, NULL);

    // prepend the the new head
    else INFO_NAMES( arg_info) = KVLLprepend(INFO_NAMES( arg_info), varname, name);

    // traverse over the nodes
    FOR_BLOCK ( arg_node) = TRAVopt ( FOR_BLOCK ( arg_node), arg_info);

    // remove the node from the list
    INFO_NAMES( arg_info) = KVLLremove_front ( INFO_NAMES( arg_info));

    // create a new vardecl node
    node *step = TBmakeVardecl ( STRcat ( name, "_step"), T_int, NULL, NULL, NULL);
    node *stop = TBmakeVardecl ( STRcat ( name, "_stop"), T_int, NULL, NULL, step);
    node *start = TBmakeVardecl ( STRcpy ( name), T_int, NULL, NULL, stop);

    // do we already have a front set?
    if ( INFO_VARDECLS (arg_info) == NULL) INFO_VARDECLS ( arg_info) = start;
    else append(INFO_VARDECLS (arg_info), start);

    // step expression
    node *stepexpr = FOR_STEP ( arg_node) ? COPYdoCopy ( FOR_STEP ( arg_node)) : TBmakeNum ( 1);
    node *assignstep = TBmakeAssign ( TBmakeVarlet ( STRcpy( VARDECL_NAME ( step)), step, NULL), stepexpr);
    node *assignstop = TBmakeAssign ( TBmakeVarlet ( STRcpy( VARDECL_NAME ( stop)), stop, NULL), COPYdoCopy ( FOR_STOP ( arg_node)));
    node *assignstart = TBmakeAssign ( TBmakeVarlet ( STRcpy( VARDECL_NAME ( start)), start, NULL), COPYdoCopy ( FOR_START ( arg_node)));

    node *stmtsstep = TBmakeStmts( assignstep, NULL);
    node *stmtsstop = TBmakeStmts( assignstop, stmtsstep);
    node *stmtsstart = TBmakeStmts( assignstart, stmtsstop);

    // remember the statments
    INFO_STMTS (arg_info) = stmtsstart;

    // copy the blocks
    node *block = COPYdoCopy ( FOR_BLOCK ( arg_node));

    // create the assignemnt statement
    node *assign = TBmakeAssign (
        TBmakeVarlet (STRcpy(VARDECL_NAME ( start)), start, NULL), 
        TBmakeBinop(BO_add, TBmakeVar (STRcpy(VARDECL_NAME ( start)), start, NULL), TBmakeVar (STRcpy( VARDECL_NAME ( step)), step, NULL))
    );

    //append the statement to the end
    if (block == NULL) block = TBmakeStmts ( assign, NULL );
    
    else append(block, TBmakeStmts ( assign, NULL ));

    // remove the node
    FREEdoFreeTree(arg_node);

    // create the conditions
    node *ternary = TBmakeTernary(
        TBmakeBinop(BO_gt, TBmakeVar (STRcpy( VARDECL_NAME ( step)), step, NULL), TBmakeNum (0)),
        TBmakeBinop(BO_lt, TBmakeVar (STRcpy( VARDECL_NAME ( start)), start, NULL), TBmakeVar (STRcpy( VARDECL_NAME ( stop)), stop, NULL)),
        TBmakeBinop(BO_gt, TBmakeVar (STRcpy( VARDECL_NAME ( start)), start, NULL), TBmakeVar (STRcpy( VARDECL_NAME ( stop)), stop, NULL))
    );

    //add the while loop
    arg_node = TBmakeWhile(
        ternary,
        block
    );

    // done
    DBUG_RETURN( arg_node);
}

node *NFLvarlet(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLvarlet");
    DBUG_PRINT ("NFL", ("NFLvarlet"));

    // search the list
    kvlistnode *node = KVLLsearch ( INFO_NAMES (arg_info), VARLET_NAME ( arg_node));

    // do we need to replace the name of the varlet?
    if (node == NULL) DBUG_RETURN( arg_node);

    // set the new name
    VARLET_NAME ( arg_node) = STRcpy(node->value);

    // done
    DBUG_RETURN( arg_node);
}

node *NFLvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLvar");
    DBUG_PRINT ("NFL", ("NFLvar"));

    // search the list
    kvlistnode *node = KVLLsearch ( INFO_NAMES (arg_info), VAR_NAME ( arg_node));

    // do we need to replace the name of the varlet?
    if (node == NULL) DBUG_RETURN( arg_node);

    // set the new name
    VAR_NAME ( arg_node) = STRcpy(node->value);

    // done
    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */
node *NFLdoNormalizeForLoops(node *syntaxtree) {
    DBUG_ENTER("NFLdoNormalizeForLoops");
    DBUG_PRINT ("NFL", ("NFLdoNormalizeForLoops"));

    srand(time(NULL));   // Initialization, should only be called once.

    info *info = MakeInfo(NULL);

    TRAVpush( TR_nfl);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
