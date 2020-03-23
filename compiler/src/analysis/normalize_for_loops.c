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


#include "normalize_for_loops.h"
#include "linked_list.h"

#include "string.h"
#include "stdio.h"
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
  int depth;
  node *front;
  node *back;
  listnode *names;
};

/*
 * INFO macros
 */

#define INFO_DEPTH(n)  ((n)->depth)
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

  INFO_DEPTH ( result) = 0;
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

node *NFLfor(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLfor");

    // remember the var name
    const char *varname = FOR_LOOPVAR (arg_node);

    // @todo 10 (used for for_%d_ part ) is arbetrary
    char *name = (char *)MEMmalloc(sizeof(char) * strlen(varname) + 10);

    // set the new name
    sprintf(name, "for_%d_%s", INFO_DEPTH (arg_info), varname);

    // copy the expression
    node *copy = COPYdoCopy(FOR_START ( arg_node));

    // create a new vardecl node
    node *vardecl = TBmakeVardecl(name, T_int, NULL, copy, NULL);

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

    // set the depth
    INFO_DEPTH (arg_info) =+ 1;

    // add the name to the list
    if (INFO_NAMES( arg_info) == NULL) INFO_NAMES( arg_info) = LLcreate(varname, name, NULL);

    // prepend the the new head
    else INFO_NAMES( arg_info) = LLprepend(INFO_NAMES( arg_info), varname, name);

    // traverse over the nodes
    TRAVopt ( FOR_BLOCK ( arg_node), arg_info);

    // remove the node from the list
    INFO_NAMES( arg_info) = LLremove_front ( INFO_NAMES( arg_info));

    // done
    DBUG_RETURN( arg_node);
}

node *NFLfunbody(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLfunbody");

    // traverse over the sons
    TRAVopt ( FUNBODY_STMTS(arg_node), arg_info);

    // do we need to add vardecls?
    node *last = INFO_BACK ( arg_info);
    if ( last == NULL) DBUG_RETURN( arg_node);

    // add the verdecls
    VARDECL_NEXT ( last) = FUNBODY_VARDECLS( arg_node);
    FUNBODY_VARDECLS ( arg_node) = INFO_FRONT ( arg_info);

    // done
    DBUG_RETURN( arg_node);
}

node *NFLvarlet(node * arg_node, info * arg_info)
{
    DBUG_ENTER("NFLvarlet");

    // search the list
    listnode *node = LLsearch ( INFO_NAMES (arg_info), VARLET_NAME ( arg_node));

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

    // search the list
    listnode *node = LLsearch ( INFO_NAMES (arg_info), VAR_NAME ( arg_node));

    // do we need to replace the name of the varlet?
    if (node != NULL) DBUG_RETURN( arg_node);
    
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

    info *info = MakeInfo(NULL);

    TRAVpush( TR_nfl);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
