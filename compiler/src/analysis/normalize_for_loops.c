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

/*
 * INFO structure
 */

struct INFO {
  node *front;
  node *back;
  kvlistnode *names;
};

/*
 * INFO macros
 */
#define INFO_FRONT(n)  ((n)->front)
#define INFO_BACK(n)  ((n)->back)
#define INFO_NAMES(n)  ((n)->names)

/*
 * INFO functions
 */

static info *MakeInfo()
{
  info *result;

  DBUG_ENTER( "MakeInfo");

  result = (info *)MEMmalloc(sizeof(info));

  INFO_FRONT ( result) = NULL;
  INFO_BACK ( result) = NULL;
  INFO_NAMES ( result) = NULL;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
  DBUG_ENTER ("FreeInfo");

  info = MEMfree( info);

  DBUG_RETURN( info);
}

node *NFLfunbody(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLfunbody");
    DBUG_PRINT ("NFL", ("NFLfunbody"));

    // traverse over the sons
    TRAVopt ( FUNBODY_STMTS(arg_node), arg_info);

    // do we need to add vardecls?
    node *front = INFO_FRONT ( arg_info);

    if ( front == NULL) DBUG_RETURN( arg_node);

    // the var decls
    node * decls = FUNBODY_VARDECLS ( arg_node);
    
    // append the var decls
    if (decls == NULL) FUNBODY_VARDECLS ( arg_node) = front;

    // add the decls
    else TRAVdo (decls, arg_info);

    // reset the variable
    INFO_FRONT ( arg_info) = NULL;

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

    // set the new name
    char *name = STRcatn ( 4, "__for_", STRitoa(index), "_" , varname);

    // copy the expression
    node *copy = COPYdoCopy ( FOR_START ( arg_node));

    // create a new vardecl node
    node *vardecl = TBmakeVardecl ( name, T_int, NULL, copy, NULL);

    // do we already have a front set?
    if ( INFO_FRONT (arg_info) == NULL)
    {
        // set front and back
        INFO_FRONT ( arg_info) = vardecl;
        INFO_BACK ( arg_info) = vardecl;
    }
    else
    {
        // set the new back
        VARDECL_NEXT ( INFO_BACK (arg_info)) = vardecl;
        INFO_BACK ( arg_info) = vardecl;
    }

    // add the name to the list
    if (INFO_NAMES( arg_info) == NULL) INFO_NAMES( arg_info) = KVLLcreate(varname, name, NULL);

    // prepend the the new head
    else INFO_NAMES( arg_info) = KVLLprepend(INFO_NAMES( arg_info), varname, name);

    // traverse over the nodes
    TRAVopt ( FOR_BLOCK ( arg_node), arg_info);

    // remove the node from the list
    INFO_NAMES( arg_info) = KVLLremove_front ( INFO_NAMES( arg_info));

    // done
    DBUG_RETURN( arg_node);
}

node *NFLvardecl ( node *arg_node, info *arg_info)
{
    DBUG_ENTER("NFLverdecl");
    DBUG_PRINT ("NFL", ("NFLverdecl"));

    // do we need to add vardecls?
    node *front = INFO_FRONT ( arg_info);
    if ( front == NULL) DBUG_RETURN( arg_node);

    // next vardecl
    node *next = VARDECL_NEXT ( arg_node);
    // do we have a next node?
    if ( VARDECL_NEXT ( arg_node) != NULL) TRAVdo (next, arg_info);

    // add the verdecls
    else VARDECL_NEXT ( arg_node) = INFO_FRONT ( arg_info);

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
