/*****************************************************************************
 *
 * Module: Type Checking
 *
 * Prefix: TC
 *
 * Description:
 *
 * @todo
 *
 *****************************************************************************/


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
#include "ctinfo.h"

/*
 * INFO structure
 */

struct INFO {
  node *table;
  type var_type;
  size_t has_return_type;
  size_t offset;
};


/*
 * INFO macros
 */

#define INFO_SYMBOL_TABLE(n)  ((n)->table)
#define INFO_VAR_TYPE(n)  ((n)->var_type)
#define INFO_HAS_RETURN_TYPE(n)  ((n)->has_return_type)
#define INFO_OFFSET(n)  ((n)->offset)


/*
 * INFO functions
 */

static info *MakeInfo()
{
  info *result;

  DBUG_ENTER( "MakeInfo");

  result = (info *)MEMmalloc(sizeof(info));
  INFO_SYMBOL_TABLE( result) = NULL;
  INFO_VAR_TYPE ( result) = T_unknown;
  INFO_HAS_RETURN_TYPE ( result) = 0;
  INFO_OFFSET ( result) = 0;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
  DBUG_ENTER ("FreeInfo");

  info = MEMfree( info);

  DBUG_RETURN( info);
}

node *TCprogram(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCprogram");
    DBUG_PRINT ("TC", ("TCprogram"));

    // link the symbol table
    INFO_SYMBOL_TABLE ( arg_info) = PROGRAM_SYMBOLTABLE ( arg_node);

    // traverse over the sons
    TRAVopt ( PROGRAM_DECLS(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *TCfundef(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCfundef");
    DBUG_PRINT ("TC", ("TCfundef"));

    // rember the table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    node *entry = STsearchFundef( table, FUNDEF_NAME ( arg_node));

    INFO_SYMBOL_TABLE ( arg_info) = SYMBOLTABLEENTRY_TABLE ( entry);

    // traverse over the body
    TRAVopt ( FUNDEF_FUNBODY ( arg_node), arg_info);

    // do we have a return statement
    if (FUNDEF_TYPE ( arg_node) != T_void && INFO_HAS_RETURN_TYPE ( arg_info) == 0 && FUNDEF_ISIMPORT(arg_node) < 1)
    {
        CTIerrorLine ( NODE_LINE ( arg_node), "No return type %s expted for %s(...)\n", stype(FUNDEF_TYPE ( arg_node)), FUNDEF_NAME ( arg_node));
    }
    

    INFO_HAS_RETURN_TYPE ( arg_info) = 0;
    INFO_SYMBOL_TABLE ( arg_info) = table;

    DBUG_RETURN( arg_node);
}

node *TCexprstmt(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCexprstmt");
    DBUG_PRINT ("TC", ("TCexprstmt"));

    TRAVopt(EXPRSTMT_EXPR( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *TCreturn(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TCreturn");
    DBUG_PRINT ("TC", ("TCreturn"));

    // remember that we have a return type
    INFO_HAS_RETURN_TYPE ( arg_info) += 1;

    // the expression
    node *expr = RETURN_EXPR ( arg_node);
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // do nothing for void methods
    if (expr == NULL && SYMBOLTABLE_RETURNTYPE ( table) == T_void) DBUG_RETURN( arg_node);

    // do the types match
    TRAVopt ( RETURN_EXPR ( arg_node), arg_info);

    // do we have the corrrect return type?
    if (INFO_VAR_TYPE ( arg_info) == SYMBOLTABLE_RETURNTYPE ( table)) DBUG_RETURN( arg_node);

    // report the error
    CTIerrorLine ( NODE_LINE ( arg_node), "invalid conversion from `%s` to `%s`\n", stype(SYMBOLTABLE_RETURNTYPE ( table)), stype(INFO_VAR_TYPE ( arg_info)));

    DBUG_RETURN( arg_node);
}

node *TCassign(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TCassign");
    DBUG_PRINT ("TC", ("TCassign"));

    // remember the type
    type type = INFO_VAR_TYPE ( arg_info);

    // get the left hand expression
    node *varlet = ASSIGN_LET ( arg_node);

    // seach for the entry
    node *node = STdeepSearchVariableByName( INFO_SYMBOL_TABLE ( arg_info), VARLET_NAME( varlet));

    // set the type 
    INFO_VAR_TYPE ( arg_info) = SYMBOLTABLEENTRY_TYPE ( node);

    // traverse over the expressions
    TRAVdo (  ASSIGN_EXPR ( arg_node), arg_info);

    // reset the type
    INFO_VAR_TYPE ( arg_info) = type;

    DBUG_RETURN( arg_node);

}

node *TCbinop (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCbinop");
    DBUG_PRINT ("TC", ("TCbinop"));

    TRAVopt ( BINOP_LEFT ( arg_node), arg_info);

    type leftype = INFO_VAR_TYPE ( arg_info);

    TRAVopt ( BINOP_RIGHT ( arg_node), arg_info);

    type righttype = INFO_VAR_TYPE ( arg_info);

    switch (BINOP_OP( arg_node))
    {
        case BO_add:
        case BO_sub:
        case BO_mul:
        case BO_div:

            if (leftype != righttype) CTIerrorLine ( NODE_LINE ( arg_node), "invalid conversion from `%s` to `%s`\n", stype(leftype), stype(righttype));

            break;
        case BO_lt:
        case BO_le:
        case BO_gt:
        case BO_ge:
        case BO_eq:
        case BO_ne:

            if (leftype != righttype) CTIerrorLine ( NODE_LINE ( arg_node), "invalid conversion from `%s` to `%s`\n", stype(leftype), stype(righttype));

            INFO_VAR_TYPE ( arg_info) = T_bool;

            break;
        case BO_and:
        case BO_or:

            if (righttype != T_bool) CTIerrorLine ( NODE_LINE ( arg_node), "invalid conversion from `%s` to `%s`\n", stype(T_bool), stype(righttype));

            break;
        case BO_mod:

            // mod operator only support intergers
            if (righttype != T_int) CTIerrorLine ( NODE_LINE ( arg_node), "Modulo operator only supports interger types\n");

            break;

        case BO_unknown:
            break;
    }

    DBUG_RETURN( arg_node);
}

node *TCfuncall(node * arg_node, info * arg_info)
{
    DBUG_ENTER ( "TCfuncall");
    DBUG_PRINT ( "TC", ("TCfuncall"));

    // get the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // get the entry
    node *entry = STdeepSearchFundef ( table, FUNCALL_NAME ( arg_node));

    // set the new symbol table
    INFO_SYMBOL_TABLE ( arg_info) = SYMBOLTABLEENTRY_TABLE ( entry);

    // traverse over the arguments
    TRAVopt ( FUNCALL_ARGS ( arg_node), arg_info);

    // restore the table
    INFO_SYMBOL_TABLE ( arg_info) = table;

    // set the type
    INFO_VAR_TYPE ( arg_info) = SYMBOLTABLEENTRY_TYPE ( entry);

    DBUG_RETURN( arg_node);
}

extern node *TCexprs (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCparam");
    DBUG_PRINT ("TC", ("TCexprs"));

    //
    TRAVdo ( EXPRS_EXPR ( arg_node), arg_info);

    // increment the number of arguments
    INFO_OFFSET ( arg_info) += 1;

    // traverse over the next expression
    TRAVopt ( EXPRS_NEXT ( arg_node), arg_info);

    INFO_OFFSET ( arg_info) -= 1;

    // done
    DBUG_RETURN( arg_node);
}

node *TCvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCvar");
    DBUG_PRINT ("TC", ("TCvar"));

    // get the entry
    node *node = STdeepSearchVariableByName( INFO_SYMBOL_TABLE ( arg_info), VAR_NAME( arg_node));

    // set the type
    INFO_VAR_TYPE ( arg_info) = SYMBOLTABLEENTRY_TYPE ( node);

    DBUG_RETURN( arg_node);
}

node *TCcast (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCcast");
    DBUG_PRINT ("TC", ("TCcast"));

    // set the type
    INFO_VAR_TYPE ( arg_info) = CAST_TYPE ( arg_node);

    DBUG_RETURN( arg_node);
}

node *TCnum (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCnum");
    DBUG_PRINT ("TC", ("TCnum"));

    // set the type
    INFO_VAR_TYPE ( arg_info) = T_int;

    DBUG_RETURN( arg_node);
}

node *TCfloat (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCfloat");
    DBUG_PRINT ("TC", ("TCfloat"));

    // set the type
    INFO_VAR_TYPE ( arg_info) = T_float;

    DBUG_RETURN( arg_node);
}

node *TCbool (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCbool");
    DBUG_PRINT ("TC", ("TCbool"));

    // set the type
    INFO_VAR_TYPE ( arg_info) = T_bool;

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */
node *TCdoTypeChecking(node *syntaxtree) {
    DBUG_ENTER("TCdoTypeChecking");
    DBUG_PRINT ("TC", ("TCdoTypeChecking"));

    info *info = MakeInfo(NULL);

    TRAVpush( TR_tc);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
