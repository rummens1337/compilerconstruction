/*****************************************************************************
 *
 * Module: Change variable initialisations
 *
 * Prefix: CVI
 *
 * Description:
 *
 * Change all variable initialisations by separating the initialisation
 * and declaration.
 * Global scope's initialisation is moved to an __init function per compilation unit.
 *
 *****************************************************************************/

#include "change_variable_initialisations.h"

#include "symbol_table.h"
#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "memory.h"
#include "free.h"
#include "str.h"
#include "ctinfo.h"
#include "copy.h"
#include "print.h"

/*
 * INFO structure
 */

struct INFO
{
    node* table;
    node *front;
    node *last;
};

/*
 * INFO macros
 */

#define INFO_FRONT(n) ((n)->front)
#define INFO_LAST(n) ((n)->last)
#define INFO_SYMBOL_TABLE(n) ((n)->table)


/*
 * INFO functions
 */

static info *MakeInfo(void)
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));

    INFO_SYMBOL_TABLE ( result) = NULL;
    INFO_FRONT ( result) = NULL;
    INFO_LAST ( result) = NULL;

    DBUG_RETURN(result);
}

static info *FreeInfo(info *info)
{
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

node *CVIprogram (node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIprogram");
    DBUG_PRINT("CVI", ("CVIprogram"));

    // set the symbol table
    INFO_SYMBOL_TABLE (arg_info) = PROGRAM_SYMBOLTABLE ( arg_node);

    // traverse over the decls
    TRAVopt ( PROGRAM_DECLS ( arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

/**
 * Traverse over decls (globdefs) and separate declaration and initialisation.
 */
node *CVIdecls(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIdecls");
    DBUG_PRINT("CVI", ("CVIdecls"));

    // traverse over the declerations
    TRAVopt(DECLS_DECL(arg_node), arg_info);
    TRAVopt(DECLS_NEXT(arg_node), arg_info);

    // get the added statements
    node *stmts = INFO_FRONT(arg_info);

    // Do we have any more declerations?
    if (DECLS_NEXT(arg_node) != NULL) DBUG_RETURN(arg_node);
    
    // do we need to append statements?
    if (stmts == NULL) DBUG_RETURN(arg_node);
    
    // create the __init function definition
    node *funbod = TBmakeFunbody(NULL, NULL, stmts);
    node *init = TBmakeFundef(T_void, STRcpy("__init"), funbod, NULL);
    FUNDEF_ISEXPORT(init) = 1;

    // prepend the __init function
    DECLS_NEXT(arg_node) = DECLS_DECL(arg_node);
    DECLS_DECL(arg_node) = init;

    // refernce to the symbol table
    node *table = INFO_SYMBOL_TABLE ( arg_info);

    // create a new symbol table for this function definition
    node *inittable = TBmakeSymboltable ( 1, NULL );
    SYMBOLTABLE_PARENT ( inittable) = table;

    // create the symbol table
    node *entry = TBmakeSymboltableentry ( STRcpy(FUNDEF_NAME ( init)), FUNDEF_TYPE ( init), 0, NULL, inittable);
    
    // add the entry to the symbol table
    STadd(table, entry);

    DBUG_RETURN(arg_node);
}

node *CVIglobdef(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIglobdef");
    DBUG_PRINT("CVI", ("CVIglobdef"));
    node *expr = GLOBDEF_INIT(arg_node);

    // do we have expressions?
    if (expr == NULL) DBUG_RETURN(arg_node);
    
    node *varlet = TBmakeVarlet(STRcpy(GLOBDEF_NAME(arg_node)), NULL, NULL);
    node *assign = TBmakeAssign(varlet, COPYdoCopy(expr));

    FREEdoFreeTree(expr);
    GLOBDEF_INIT(arg_node) = NULL;

    if (INFO_FRONT(arg_info) == NULL)
    {
        INFO_FRONT(arg_info) = TBmakeStmts(assign, NULL);
        INFO_LAST(arg_info) = INFO_FRONT(arg_info);
    }
    else
    {
        node *node = TBmakeStmts(assign, NULL);
        STMTS_NEXT(INFO_LAST(arg_info)) = node;
        INFO_LAST(arg_info) = node;
    }

    DBUG_RETURN(arg_node);
}

/**
 * Traverse over funbodies and separate declaration and initialisation of vardecls
 * that have an initilisation.
 */
node *CVIfunbody(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIfunbody");
    DBUG_PRINT("CVI", ("CVIfunbody"));

    info *info = MakeInfo();
    TRAVopt(FUNBODY_VARDECLS(arg_node), info);

    if (INFO_LAST(info) == NULL) DBUG_RETURN(arg_node);

    STMTS_NEXT(INFO_LAST(info)) = FUNBODY_STMTS(arg_node);
    FUNBODY_STMTS(arg_node) = INFO_FRONT(info);

    FreeInfo(info);
    DBUG_RETURN(arg_node);
}

node *CVIvardecl(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIvardecl");
    DBUG_PRINT("CVI", ("CVIvardecl"));

    node *expr = VARDECL_INIT ( arg_node);

    // do we have expressions?
    if (expr == NULL) DBUG_RETURN(arg_node);
    
    node *varlet = TBmakeVarlet(STRcpy(VARDECL_NAME(arg_node)), NULL, NULL);
    node *assign = TBmakeAssign(varlet, COPYdoCopy(expr));

    FREEdoFreeTree(expr);
    VARDECL_INIT(arg_node) = NULL;

    if (INFO_FRONT(arg_info) == NULL)
    {
        INFO_FRONT(arg_info) = TBmakeStmts(assign, NULL);
        INFO_LAST(arg_info) = INFO_FRONT(arg_info);
    }
    else
    {
        node *node = TBmakeStmts(assign, NULL);
        STMTS_NEXT(INFO_LAST(arg_info)) = node;
        INFO_LAST(arg_info) = node;
    }

    TRAVopt(VARDECL_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

/*
 * Traversal start function
 */
node *CVIdoChangeVariableInitialisations(node *syntaxtree)
{
    DBUG_ENTER("CVIdoChangeVariableInitialisations");
    DBUG_PRINT("CVI", ("CVIdoChangeVariableInitialisations"));

    info *info = MakeInfo();

    TRAVpush(TR_cvi);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
