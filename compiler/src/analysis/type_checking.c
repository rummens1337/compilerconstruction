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
  type return_type;
  int offset;
  int arguments;
  size_t has_return_type;
};


/*
 * INFO macros
 */

#define INFO_SYMBOL_TABLE(n)  ((n)->table)
#define INFO_VAR_TYPE(n)  ((n)->var_type)
#define INFO_RETURN_TYPE(n)  ((n)->var_type)
#define INFO_OFFSET(n)  ((n)->offset)
#define INFO_ARGUMENTS(n)  ((n)->arguments)
#define INFO_HAS_RETURN_TYPE(n)  ((n)->has_return_type)


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
  INFO_RETURN_TYPE ( result) = T_unknown;
  INFO_OFFSET ( result) = 0;
  INFO_HAS_RETURN_TYPE ( result) = 0;
  INFO_ARGUMENTS ( result) = 0;


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

    //
    if (entry == NULL) DBUG_RETURN( arg_node);

    INFO_SYMBOL_TABLE ( arg_info) = SYMBOLTABLEENTRY_TABLE ( entry);

    // remember the type
    INFO_RETURN_TYPE( arg_info) = FUNDEF_TYPE ( arg_node);

    // traverse over the body
    TRAVopt ( FUNDEF_FUNBODY ( arg_node), arg_info);

    // do we have a return statement
    if (FUNDEF_TYPE ( arg_node) != T_void && INFO_HAS_RETURN_TYPE ( arg_info) == 0)
    {
        CTIerror ("No return type %s expted for %s(...)\n", stype(FUNDEF_TYPE ( arg_node)), FUNDEF_NAME ( arg_node));
    }

    INFO_HAS_RETURN_TYPE ( arg_info) = 0;
    INFO_SYMBOL_TABLE ( arg_info) = table;

    DBUG_RETURN( arg_node);
}

node *TCexprstmt(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCexprstmt");
    DBUG_PRINT ("TC", ("TCexprstmt"));

    // @todo do type checking for other expressions

    // do we have a return type?
    if ( INFO_RETURN_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);

    TRAVopt(EXPRSTMT_EXPR( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *TCreturn(node *arg_node, info *arg_info)
{
    DBUG_ENTER("TCreturn");
    DBUG_PRINT ("TC", ("TCreturn"));

    // remember that we have a return type
    INFO_HAS_RETURN_TYPE ( arg_info) += 1;

    // do nothing if we dont have a type
    if (INFO_RETURN_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);

    // check void types
    // if (INFO_RETURN_TYPE ( arg_info) == T_void && RETURN_EXPR ( arg_node) != NULL)
    // {
    //     CTIerror ("No return type expted\n");
    // }

    // remember the type
    type type = INFO_VAR_TYPE ( arg_info);

    // update the type
    INFO_VAR_TYPE ( arg_info) = INFO_RETURN_TYPE ( arg_info);

    // do the types match
    TRAVopt (RETURN_EXPR ( arg_node), arg_info);

    // reset the type
    INFO_VAR_TYPE ( arg_info) = type;

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

node *TCvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCvar");
    DBUG_PRINT ("TC", ("TCvar"));

    type type = INFO_VAR_TYPE ( arg_info);

    // do nothing if we dont have a type
    if (type == T_unknown) DBUG_RETURN( arg_node);

    // get the entry
    node *node = STsearchVariableByName( INFO_SYMBOL_TABLE ( arg_info), VAR_NAME( arg_node));

    // is the type correct?
    if (type == SYMBOLTABLEENTRY_TYPE ( node)) DBUG_RETURN( arg_node);

    // report the error
    CTIerror ("invalid conversion from `%s` to `%s`\n", stype(type), stype(SYMBOLTABLEENTRY_TYPE ( node)));

    DBUG_RETURN( arg_node);
}

node *TCbinop (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCbinop");
    DBUG_PRINT ("TC", ("TCbinop"));

    // do nothing if we dont have a type
    if (INFO_VAR_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);
    switch (BINOP_OP( arg_node))
    {
        case BO_add:
        case BO_sub:
        case BO_mul:
        case BO_div:
            if (INFO_VAR_TYPE ( arg_info) != T_bool) CTIerror ("invalid conversion from `%s` to `%s`\n", stype(T_bool), stype(INFO_VAR_TYPE ( arg_info)));
            else 
            {
                INFO_VAR_TYPE ( arg_info) = T_int;
                TRAVopt ( BINOP_LEFT ( arg_node), arg_info);
                TRAVopt ( BINOP_RIGHT ( arg_node), arg_info);
            }
            break;
        case BO_mod:
            // mod operator only support intergers
            if (INFO_VAR_TYPE ( arg_info) != T_int) CTIerror("Modulo operator only supports interger types\n");
            break;
        case BO_lt:
        case BO_le:
        case BO_gt:
        case BO_ge:
        case BO_eq:
        case BO_ne:
        case BO_and:
        case BO_or:
        case BO_unknown:
            INFO_VAR_TYPE ( arg_info) =  T_unknown;
            break;
    }

    DBUG_RETURN( arg_node);
}

node *TCnum (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCnum");
    DBUG_PRINT ("TC", ("TCnum"));

    // do nothing if we dont have a type
    if (INFO_VAR_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);

    // do the types match
    if (INFO_VAR_TYPE (arg_info) == T_int) DBUG_RETURN( arg_node);

    // report the error
    CTIerror ("invalid conversion from `%s` to `%s`\n", stype(T_int), stype(INFO_VAR_TYPE (arg_info)));

    DBUG_RETURN( arg_node);
}

node *TCfloat (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCfloat");
    DBUG_PRINT ("TC", ("TCfloat"));

    // do nothing if we dont have a type
    if (INFO_VAR_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);

    // do the types match
    if (INFO_VAR_TYPE (arg_info) == T_float) DBUG_RETURN( arg_node);

    // report the error
    CTIerror ("invalid conversion from `%s` to `%s`\n", stype(T_float), stype(INFO_VAR_TYPE (arg_info)));

    DBUG_RETURN( arg_node);
}

node *TCbool (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCbool");
    DBUG_PRINT ("TC", ("TCbool"));

    // do nothing if we dont have a type
    if (INFO_VAR_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);

    // do the types match
    if (INFO_VAR_TYPE (arg_info) == T_bool) DBUG_RETURN( arg_node);

    // report the error
    CTIerror ("invalid conversion from `%s` to `%s`\n", stype(T_bool), stype(INFO_VAR_TYPE (arg_info)));

    DBUG_RETURN( arg_node);
}

node *TCcast (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCcast");
    DBUG_PRINT ("TC", ("TCcast"));

    // do nothing if we dont have a type
    if (INFO_VAR_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);

    // do the types match
    if (CAST_TYPE ( arg_node) == INFO_VAR_TYPE (arg_info)) DBUG_RETURN( arg_node);

    // report the error
    CTIerror ("invalid conversion from `%s` to `%s`\n", stype(CAST_TYPE ( arg_node)), stype(INFO_VAR_TYPE (arg_info)));

    DBUG_RETURN( arg_node);
}

node *TCfuncall(node * arg_node, info * arg_info)
{
    DBUG_ENTER ( "TCfuncall");
    DBUG_PRINT ( "TC", ("TCfuncall"));

    // get the parameters
    node *args = FUNCALL_ARGS ( arg_node);

    // get the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // @todo should we check whether we are dealing with a function definition?
    // get the entry
    node *entry = STdeepSearchFundef ( table, FUNCALL_NAME ( arg_node));

    // do we have paramters
    if (args != NULL)
    {
        INFO_SYMBOL_TABLE ( arg_info) = SYMBOLTABLEENTRY_TABLE ( entry);
        int arguments = INFO_ARGUMENTS ( arg_info);

        // traverse over the arguments
        TRAVopt ( args, arg_info);

        // do we have the right number of arguments
        if (SYMBOLTABLEENTRY_PARAMS ( entry) > INFO_ARGUMENTS ( arg_info))
        {
           CTIerror ("Too few arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
        }
        else
        {
            CTIerror ("Too many arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
        }

        // restore the info properties
        INFO_ARGUMENTS ( arg_info) = arguments;
        INFO_SYMBOL_TABLE ( arg_info) = table;
    }

    else if (SYMBOLTABLEENTRY_PARAMS ( entry) > 0)
    {
        CTIerror ("Too few arguments to function `%s %s(...)`\n", stype(SYMBOLTABLEENTRY_TYPE ( entry)), FUNCALL_NAME ( arg_node));
    }

    // do nothing if we dont have a type
    if (INFO_VAR_TYPE ( arg_info) == T_unknown) DBUG_RETURN( arg_node);

    // is the type correct?
    if (INFO_VAR_TYPE ( arg_info) == SYMBOLTABLEENTRY_TYPE ( entry)) DBUG_RETURN( arg_node);

    // print the error
    CTIerror ("Invalid function call %s() returns %s while %s was expected\n", FUNCALL_NAME ( arg_node), stype(SYMBOLTABLEENTRY_TYPE ( entry)), INFO_VAR_TYPE ( arg_info));

    DBUG_RETURN( arg_node);
}

extern node *TCexprs (node * arg_node, info * arg_info)
{
    DBUG_ENTER("TCparam");
    DBUG_PRINT ("TC", ("TCexprs"));

    // increment the number of arguments
    INFO_ARGUMENTS ( arg_info) += 1;

    // remember the type
    type type = INFO_VAR_TYPE ( arg_info);

    // seach for the entry
    node *node = STget( INFO_SYMBOL_TABLE ( arg_info), INFO_OFFSET ( arg_info));

    // set the type 
    INFO_VAR_TYPE ( arg_info) = SYMBOLTABLEENTRY_TYPE ( node);

    // traverse over the expression
    TRAVdo ( EXPRS_EXPR ( arg_node), arg_info);

    // increment the offset
    INFO_OFFSET ( arg_info) += 1;

    // traverse over the next expression
    TRAVopt ( EXPRS_NEXT ( arg_node), arg_info);

    // decrement the offset
    INFO_OFFSET ( arg_info) -= 1;

    // reset the type
    INFO_VAR_TYPE ( arg_info) = type;

    // done
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
