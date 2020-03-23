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

#include "memory.h"
#include "free.h"
#include "str.h"
#include "ctinfo.h"

/*
 * INFO structure
 */

struct INFO {
  node *table;
};


/*
 * INFO macros
 */

#define INFO_SYMBOL_TABLE(n)  ((n)->table)


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
  SYMBOLTABLE_PARENT ( table) = parent;
  INFO_SYMBOL_TABLE( result) = table;

  DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
  DBUG_ENTER ("FreeInfo");

  info = MEMfree( info);

  DBUG_RETURN( info);
}

/*
 * Traversal functions
 */
node *PSTarrexpr(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTarrexpr");
    DBUG_PRINT ("PST", ("PSTarrexpr"));
    DBUG_RETURN( arg_node);
}

node *PSTassign(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTassign");
    DBUG_PRINT ("PST", ("PSTassign"));

    // traverse sons
    TRAVopt (ASSIGN_LET( arg_node), arg_info);
    TRAVopt (ASSIGN_EXPR( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTbinop(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTbinop");
    DBUG_PRINT ("PST", ("PSTbinop"));

    // traverse sons
    TRAVopt (BINOP_LEFT( arg_node), arg_info);
    TRAVopt (BINOP_RIGHT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTbool(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTbool");
    DBUG_PRINT ("PST", ("PSTbool"));
    DBUG_RETURN( arg_node);
}

node *PSTcast(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTcast");
    DBUG_PRINT ("PST", ("PSTcast"));

    // traverse sons
    TRAVopt (CAST_EXPR( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTdecls(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTdecls");
    DBUG_PRINT ("PST", ("PSTdecls"));

    // traverse over the sons
    TRAVopt ( DECLS_DECL(arg_node), arg_info);
    TRAVopt ( DECLS_NEXT(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTdowhile(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTdowhile");
    DBUG_PRINT ("PST", ("PSTdowhile"));

    // traverse over the sons
    TRAVopt ( DOWHILE_BLOCK(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTerror(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTerror");
    DBUG_PRINT ("PST", ("PSTerror"));
    DBUG_RETURN( arg_node);
}

node *PSTexprstmt(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTexprstmt");
    DBUG_PRINT ("PST", ("PSTexprstmt"));

    // traverse sons
    TRAVopt (EXPRSTMT_EXPR( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTexprs(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTexprs");
    DBUG_PRINT ("PST", ("PSTexprs"));

    // traverse sons
    TRAVopt (EXPRS_EXPR( arg_node), arg_info);
    TRAVopt (EXPRS_NEXT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTfloat(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfloat");
    DBUG_PRINT ("PST", ("PSTfloat"));
    DBUG_RETURN( arg_node);
}

node *PSTfor(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfor");
    DBUG_PRINT ("PST", ("PSTfor"));

    // traverse over the sons
    TRAVopt ( FOR_BLOCK(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTfunbody(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfunbody");
    DBUG_PRINT ("PST", ("PSTfunbody"));

    // traverse over the sons
    TRAVopt ( FUNBODY_VARDECLS(arg_node), arg_info);
    TRAVopt ( FUNBODY_STMTS(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTfuncall(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfuncall");
    DBUG_PRINT ("PST", ("PSTfuncall"));

    // add to the current scope
    if (!STdeepSearchByName(INFO_SYMBOL_TABLE ( arg_info), VAR_NAME ( arg_node)))
    {
        DBUG_ASSERT (0, STRcat ( "Undefined function ", VAR_NAME ( arg_node)));
    }
   
    DBUG_RETURN( arg_node);
}

node *PSTfundef(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTfundef");
    DBUG_PRINT ("PST", ("PSTfundef 1"));

    DBUG_PRINT ("PST", ("PSTfundef 2"));

    // the current table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // make a new struct for the coming scope
    info *info = MakeInfo(table);
    DBUG_PRINT ("PST", ("PSTfundef 3"));

    // create the entry
    node *entry = TBmakeSymboltableentry(FUNDEF_NAME ( arg_node), FUNDEF_TYPE ( arg_node), SToffset(table) + 1, arg_node, NULL, INFO_SYMBOL_TABLE ( info));
    DBUG_PRINT ("PST", ("PSTfundef 4"));

    // add to the current scope
    if (!STadd(table, entry))
    {
        DBUG_ASSERT (0, STRcat ( "Function name invalid ", FUNDEF_NAME ( arg_node)));
    }

    DBUG_PRINT ("PST", ("PSTfundef 5"));

    // traverse over the sons
    TRAVopt ( FUNDEF_PARAMS( arg_node), info);
    DBUG_PRINT ("PST", ("PSTfundef 6"));

    TRAVopt ( FUNDEF_FUNBODY( arg_node), info);
    DBUG_PRINT ("PST", ("PSTfundef 7"));

    DBUG_RETURN( arg_node);
}

node *PSTfundefs(node * arg_node, info * arg_info)
{
    // enter
    DBUG_ENTER("PSTfundefs");
    DBUG_PRINT ("PST", ("PSTfundefs"));

    // traverse over the sons
    TRAVopt ( FUNDEFS_FUNDEF(arg_node), arg_info);
    TRAVopt ( FUNDEFS_NEXT(arg_node), arg_info);


    // done
    DBUG_RETURN( arg_node);
}

node *PSTglobdecl(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTglobdecl");
    DBUG_PRINT ("PST", ("PSTglobdecl"));
    DBUG_RETURN( arg_node);
}

node *PSTglobdef(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTglobdef");
    DBUG_PRINT ("PST", ("PSTglobdef"));

    // the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // create the entry
    node *entry = TBmakeSymboltableentry(GLOBDEF_NAME ( arg_node), GLOBDEF_TYPE ( arg_node), SToffset(table) + 1, arg_node, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry))
    {
        DBUG_ASSERT (0, STRcat ( "Variable already declared ", GLOBDEF_NAME ( arg_node)));
    }

    DBUG_RETURN( arg_node);
}

node *PSTids(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTids");
    DBUG_PRINT ("PST", ("PSTids"));

    // traverse over the sons
    TRAVopt ( IDS_NEXT(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTifelse(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTifelse");
    DBUG_PRINT ("PST", ("PSTifelse"));

    // traverse over the sons
    TRAVopt ( IFELSE_THEN(arg_node), arg_info);
    TRAVopt ( IFELSE_ELSE(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTmonop(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTmonop");
    DBUG_PRINT ("PST", ("PSTmonop"));
    DBUG_RETURN( arg_node);
}

node *PSTnum(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTnum");
    DBUG_PRINT ("PST", ("PSTnum"));
    DBUG_RETURN( arg_node);
}

node *PSTparam(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTparam");
    DBUG_PRINT ("PST", ("PSTparam"));

    // the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // create the entry
    node *entry = TBmakeSymboltableentry(PARAM_NAME ( arg_node), PARAM_TYPE ( arg_node), SToffset(table) + 1, arg_node, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry))
    {
        DBUG_ASSERT (0, STRcat ( "Parameter already declared ", PARAM_NAME ( arg_node)));
    }

    // traverse over the sons
    TRAVopt ( PARAM_NEXT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
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

node *PSTreturn(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTreturn");
    DBUG_PRINT ("PST", ("PSTreturn"));
    DBUG_RETURN( arg_node);
}

node *PSTstmts(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTstmts");
    DBUG_PRINT ("PST", ("PSTstmts"));

    // traverse over the sons
    TRAVopt ( STMTS_STMT(arg_node), arg_info);
    TRAVopt ( STMTS_NEXT(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTvar(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvar");
    DBUG_PRINT ("PST", ("PSTvar"));

    // add to the current scope
    if (!STdeepSearchByName(INFO_SYMBOL_TABLE ( arg_info), VAR_NAME ( arg_node)))
    {
        DBUG_ASSERT (0, STRcat ( "Undefined variable ", VAR_NAME ( arg_node)));
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
    node *entry = TBmakeSymboltableentry ( VARDECL_NAME ( arg_node), VARDECL_TYPE ( arg_node), SToffset(table) + 1, arg_node, NULL, NULL);

    // add to the current scope
    if (!STadd(table, entry))
    {
        DBUG_ASSERT (0, STRcat ( "Variable already declared ", VARDECL_NAME ( arg_node)));
    }

    // traverse over the sons
    TRAVopt ( VARDECL_NEXT( arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTvarlet(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTvarlet");
    DBUG_PRINT ("PST", ("PSTvarlet"));

    // add to the current scope
    if (!STdeepSearchByName(INFO_SYMBOL_TABLE ( arg_info), VARLET_NAME ( arg_node)))
    {
        DBUG_ASSERT (0, STRcat ( "Undefined variable ", VARLET_NAME ( arg_node)));
    }

    DBUG_RETURN( arg_node);
}

node *PSTwhile(node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTwhile");
    DBUG_PRINT ("PST", ("PSTwhile"));

    // traverse over the sons
    TRAVopt ( WHILE_BLOCK(arg_node), arg_info);

    DBUG_RETURN( arg_node);
}

node *PSTsymboltable (node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTsymboltable");
    DBUG_PRINT ("PST", ("PSTsymboltable"));
    DBUG_RETURN( arg_node);
}

node *PSTsymboltableentry (node * arg_node, info * arg_info)
{
    DBUG_ENTER("PSTsymboltableentry");
    DBUG_PRINT ("PST", ("PSTsymboltableentry"));
    DBUG_RETURN( arg_node);
}

static void printSymbolTable(void *value)
{
    node *entry = (node*)value;

    printf("Type: ");
    switch (SYMBOLTABLEENTRY_TYPE ( entry)) {
    case T_void:
      printf("void");
      break;
    case T_bool:
      printf("bool");
      break;
    case T_int:
      printf("int");
      break;
    case T_float:
      printf("float");
      break;
    case T_unknown:
      DBUG_ASSERT( 0, "unknown type detected!");
  }

  printf(", Name: %s\n", SYMBOLTABLEENTRY_NAME ( entry));
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

    STtraverse( INFO_SYMBOL_TABLE(info), printSymbolTable);

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
