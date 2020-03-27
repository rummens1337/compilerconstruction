/*****************************************************************************
 *
 * Module: process symbol table
 *
 * Prefix: PST
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/


#include "process_symbol_table.h"
#include "symbol_table.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"
#include "print.h"
#include "memory.h"
#include "free.h"
#include "str.h"
#include "ctinfo.h"


/*
 * INFO structure
 */

struct INFO {
  node *table;
  int params;
  int arguments;
};


/*
 * INFO macros
 */

#define INFO_SYMBOL_TABLE(n)  ((n)->table)
#define INFO_PARAMS(n)  ((n)->params)
#define INFO_ARGUMENTS(n)  ((n)->arguments)



/*
 * INFO functions
 */

static info *MakeInfo(node *parent)
{
  info *result;

  DBUG_ENTER( "MakeInfo");

  result = (info *)MEMmalloc(sizeof(info));

  int distance = parent ? SYMBOLTABLE_DISTANCE ( parent) + 1 : 0;

  node *table = TBmakeSymboltable(distance, NULL);
  SYMBOLTABLE_RETURNTYPE ( table) = T_unknown;
  SYMBOLTABLE_PARENT ( table) = parent;
  INFO_SYMBOL_TABLE( result) = table;
  INFO_PARAMS ( result) = 0;
  INFO_ARGUMENTS ( result) = 0;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
  DBUG_ENTER ("FreeInfo");

  info = MEMfree( info);

  DBUG_RETURN( info);
}

node *PSTprogram(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTprogram");
    DBUG_PRINT ("PST", ("PSTprogram"));

    // link the symbol table
    PROGRAM_SYMBOLTABLE ( arg_node) = INFO_SYMBOL_TABLE ( arg_info);

    // traverse over the sons
    TRAVopt ( PROGRAM_DECLS(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTglobdef(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTglobdef");
    DBUG_PRINT ("PST", ("PSTglobdef"));

    // the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // create the entry
    node *entry = TBmakeSymboltableentry ( GLOBDEF_NAME ( arg_node), GLOBDEF_TYPE ( arg_node), 0, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry)) CTIerror ("Multiple definition of `%s'\n", GLOBDEF_NAME ( arg_node));

    DBUG_RETURN( arg_node);
}

node *PSTfundef(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfundef");
    DBUG_PRINT ("PST", ("PSTglobdef"));

    // the current table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // make a new struct for the coming scope
    info *info = MakeInfo(table);
    SYMBOLTABLE_RETURNTYPE ( INFO_SYMBOL_TABLE ( info)) = FUNDEF_TYPE ( arg_node);

    // create the entry
    node *entry = TBmakeSymboltableentry(FUNDEF_NAME ( arg_node), FUNDEF_TYPE ( arg_node), 0, NULL, INFO_SYMBOL_TABLE ( info));

    // add to the current scope
    if (!STadd(table, entry)) CTIerror ("Multiple definition of `%s(...)'\n", FUNDEF_NAME ( arg_node));

    // traverse over the sons
    TRAVopt ( FUNDEF_PARAMS( arg_node), info);

    SYMBOLTABLEENTRY_PARAMS( entry) = INFO_PARAMS ( info);

    TRAVopt ( FUNDEF_FUNBODY( arg_node), info);

    // free the info
    FreeInfo ( info);

    DBUG_RETURN( arg_node);
}

node *PSTparam(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTparam");
    DBUG_PRINT ("PST", ("PSTparam"));

    // the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // increment the number of params
    INFO_PARAMS ( arg_info) += 1;

    // create the entry
    node *entry = TBmakeSymboltableentry( STRcpy(PARAM_NAME ( arg_node)), PARAM_TYPE ( arg_node), 0, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry)) CTIerror ( "Redefinition of `%s %s` ", stype(PARAM_TYPE ( arg_node)), PARAM_NAME ( arg_node));

    // traverse over the sons
    TRAVopt ( PARAM_NEXT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTfuncall(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfuncall");
    DBUG_PRINT ("PST", ("PSTfuncall"));

    // get the parameters
    node *args = FUNCALL_ARGS ( arg_node);

    // get the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // get the entry
    node *entry = STdeepSearchFundef ( table, FUNCALL_NAME ( arg_node));

    // add to the current scope
    if (entry == NULL) CTIerror ("`%s()` was not declared in this scope\n", FUNCALL_NAME ( arg_node));

    // do we have paramters
    else if (args != NULL)
    {
        INFO_SYMBOL_TABLE ( arg_info) = SYMBOLTABLEENTRY_TABLE ( entry);
        int backarguments = INFO_ARGUMENTS ( arg_info);

        // traverse over the arguments
        TRAVopt ( args, arg_info);

        // do we have the right number of arguments
        if (INFO_ARGUMENTS ( arg_info) < SYMBOLTABLEENTRY_PARAMS ( entry))
        {
           CTIerror ("Too few arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
        }
        else if (INFO_ARGUMENTS ( arg_info) > SYMBOLTABLEENTRY_PARAMS ( entry))
        {
            CTIerror ("Too many arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
        }

        // restore the info properties
        INFO_ARGUMENTS ( arg_info) = backarguments;
        INFO_SYMBOL_TABLE ( arg_info) = table;
    }

    else if (SYMBOLTABLEENTRY_PARAMS ( entry) > 0)
    {
        CTIerror ("Too few arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
    }
   
    DBUG_RETURN( arg_node);
}

node *PSTexprs (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCparam");
    DBUG_PRINT ("TC", ("TCexprs"));

    // increment the number of arguments
    INFO_ARGUMENTS ( arg_info) += 1;

    // traverse over the next expression
    TRAVopt ( EXPRS_NEXT ( arg_node), arg_info);

    // done
    DBUG_RETURN( arg_node);
}

node *PSTvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvar");
    DBUG_PRINT ("PST", ("PSTvar"));

    // add to the current scope
    if (!STdeepSearchVariableByName(INFO_SYMBOL_TABLE ( arg_info), VAR_NAME ( arg_node)))
    {
        CTIerror ("`%s` was not declared in this scope\n", VAR_NAME ( arg_node));
    }

    DBUG_RETURN( arg_node);
}

node *PSTvardecl(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvardecl");
    DBUG_PRINT ("PST", ("PSTvardecl"));

    // the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // create the entry
    node *entry = TBmakeSymboltableentry ( VARDECL_NAME ( arg_node), VARDECL_TYPE ( arg_node), 0, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry)) CTIerror ("Multiple definition of `%s'\n", VARDECL_NAME ( arg_node));

    // traverse over the sons
    TRAVopt ( VARDECL_NEXT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTvarlet(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvarlet");
    DBUG_PRINT ("PST", ("PSTvarlet"));

    // add to the current scope
    if (!STdeepSearchVariableByName(INFO_SYMBOL_TABLE ( arg_info), VARLET_NAME ( arg_node)))
    {
        CTIerror ("`%s` was not declared in this scope\n", VARLET_NAME ( arg_node));
    }

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */
node *PSTdoProcessSymbolTable(node *syntaxtree) {
    DBUG_ENTER("PSTdoProcessSymbolTable");
    DBUG_PRINT ("PST", ("PSTdoProcessSymbolTable"));

    info *info = MakeInfo(NULL);

    TRAVpush( TR_pst);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    STdisplay( INFO_SYMBOL_TABLE ( info), 0);

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
