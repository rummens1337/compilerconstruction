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

  node *table = TBmakeSymboltable(NULL);
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
    PROGRAM_DECLS(arg_node) = TRAVopt ( PROGRAM_DECLS(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTglobdef(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTglobdef");
    DBUG_PRINT ("PST", ("PSTglobdef"));

    // the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // create the entry
    node *entry = TBmakeSymboltableentry ( STRcpy(GLOBDEF_NAME ( arg_node)), GLOBDEF_TYPE ( arg_node), 0, 0, arg_node, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry)) CTIerrorLine ( NODE_LINE ( arg_node), "Multiple definition of `%s'\n", GLOBDEF_NAME ( arg_node));

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
    node *entry = TBmakeSymboltableentry( STRcpy(FUNDEF_NAME ( arg_node)), FUNDEF_TYPE ( arg_node), 0, 0, arg_node, NULL, INFO_SYMBOL_TABLE ( info));

    // add to the current scope
    if (!STadd(table, entry)) CTIerrorLine ( NODE_LINE ( arg_node), "Multiple definition of `%s(...)'\n", FUNDEF_NAME ( arg_node));

    // traverse over the sons
    FUNDEF_PARAMS( arg_node) = TRAVopt ( FUNDEF_PARAMS( arg_node), info);
    FUNDEF_FUNBODY( arg_node) = TRAVopt ( FUNDEF_FUNBODY( arg_node), info);

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

    // create the entry
    node *entry = TBmakeSymboltableentry ( STRcpy(PARAM_NAME ( arg_node)), PARAM_TYPE ( arg_node), 0, 1, arg_node, NULL, NULL);
    SYMBOLTABLEENTRY_PARAM ( entry) = TRUE;

    // add to the current scope
    if (!STadd(table, entry)) CTIerrorLine ( NODE_LINE ( arg_node),  "Redefinition of `%s %s` ", stype(PARAM_TYPE ( arg_node)), PARAM_NAME ( arg_node));

    // traverse over the sons
    PARAM_NEXT( arg_node) = TRAVopt ( PARAM_NEXT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTfuncall(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfuncall");
    DBUG_PRINT ("PST", ("PSTfuncall"));

    // get the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // get the entry
    node *entry = STdeepSearchFundef ( table, FUNCALL_NAME ( arg_node));

    // add to the current scope
    if (entry == NULL) CTIerrorLine ( NODE_LINE ( arg_node), "`%s()` was not declared in this scope\n", FUNCALL_NAME ( arg_node));

    // set the decl
    else 
    {
        FUNCALL_DECL ( arg_node) = SYMBOLTABLEENTRY_LINK(entry);

        // do we have paramters
        if (FUNCALL_ARGS ( arg_node) != NULL)
        {
            int backarguments = INFO_ARGUMENTS ( arg_info);
            INFO_ARGUMENTS ( arg_info) = 0;

            // traverse over the arguments
            FUNCALL_ARGS ( arg_node) = TRAVopt ( FUNCALL_ARGS ( arg_node), arg_info);

            // number of parameters
            size_t params = STparams ( SYMBOLTABLEENTRY_TABLE ( entry));

            // do we have the right number of arguments
            if (INFO_ARGUMENTS ( arg_info) < params)
            {
            CTIerrorLine ( NODE_LINE ( arg_node), "Too few arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
            }
            else if (INFO_ARGUMENTS ( arg_info) > params)
            {
                CTIerrorLine ( NODE_LINE ( arg_node), "Too many arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
            }

            // restore the info properties
            INFO_ARGUMENTS ( arg_info) = backarguments;
        }

        else if ( STparams ( SYMBOLTABLEENTRY_TABLE ( entry)) > 0)
        {
            CTIerrorLine ( NODE_LINE ( arg_node), "Too few arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
        }
    }
   
    DBUG_RETURN( arg_node);
}

node *PSTexprs (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCparam");
    DBUG_PRINT ("TC", ("TCexprs"));

    // increment the number of arguments
    INFO_ARGUMENTS ( arg_info) += 1;

    EXPRS_EXPR ( arg_node) = TRAVdo ( EXPRS_EXPR ( arg_node), arg_info);

    // traverse over the next expression
    EXPRS_NEXT ( arg_node) = TRAVopt ( EXPRS_NEXT ( arg_node), arg_info);

    // done
    DBUG_RETURN( arg_node);
}

node *PSTvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvar");
    DBUG_PRINT ("PST", ("PSTvar"));

    // get the entry
    node *entry = STdeepSearchVariableByName(INFO_SYMBOL_TABLE ( arg_info), VAR_NAME ( arg_node));

    // add to the current scope
    if (entry == NULL)
    {
        CTIerrorLine ( NODE_LINE ( arg_node), "`%s` was not declared in this scope\n", VAR_NAME ( arg_node));
    }
    else
    {
        // set the decl
        VAR_DECL ( arg_node) = SYMBOLTABLEENTRY_LINK(entry);
    }

    DBUG_RETURN( arg_node);
}

node *PSTvardecl(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvardecl");
    DBUG_PRINT ("PST", ("PSTvardecl"));

    // the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // traverse over the nodes, do this before adding the vardecl
    if (VARDECL_INIT ( arg_node)) VARDECL_INIT ( arg_node) = TRAVopt(VARDECL_INIT ( arg_node), arg_info);

    // create the entry
    node *entry = TBmakeSymboltableentry ( STRcpy(VARDECL_NAME ( arg_node)), VARDECL_TYPE ( arg_node), 0, 1, arg_node, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry)) CTIerrorLine ( NODE_LINE ( arg_node), "Multiple definition of `%s'\n", VARDECL_NAME ( arg_node));

    // traverse over the sons
    VARDECL_NEXT( arg_node) = TRAVopt ( VARDECL_NEXT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTvarlet(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvarlet");
    DBUG_PRINT ("PST", ("PSTvarlet"));

    // get the entry
    node *entry = STdeepSearchVariableByName(INFO_SYMBOL_TABLE ( arg_info), VARLET_NAME ( arg_node));

    // add to the current scope
    if (entry == NULL)
    {
        CTIerrorLine ( NODE_LINE ( arg_node), "`%s` was not declared in this scope\n", VARLET_NAME ( arg_node));
    }
    else
    {
        // set the decl
        VARLET_DECL ( arg_node) = SYMBOLTABLEENTRY_LINK(entry);
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
