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
 *
 *****************************************************************************/

#include "change_variable_initialisations.h"

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

struct INFO
{
    node *stmts;
    node *last;
};

/*
 * INFO macros
 */

#define INFO_STMTS(n) ((n)->stmts)
#define INFO_LAST(n) ((n)->last)

/*
 * INFO functions
 */

static info *MakeInfo(void)
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));

    INFO_STMTS(result) = NULL;
    INFO_LAST(result) = NULL;

    DBUG_RETURN(result);
}

static info *FreeInfo(info *info)
{
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

node *CVIdecls(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIdecls");
    // DBUG_PRINT ("PST", ("PSTglobdecl"));
    DBUG_RETURN(arg_node);
}

node *CVIfunbody(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIfunbody");

    info *info = MakeInfo();
    TRAVopt(FUNBODY_VARDECLS(arg_node), info);

    // stmts += FUNBODY_STMTS(arg_node);
    STMTS_NEXT(INFO_LAST(info)) = FUNBODY_STMTS(arg_node);
    FUNBODY_STMTS(arg_node) = INFO_STMTS(info);

    // TRAVopt(FUNBODY_STMTS(arg_node), info);

    FreeInfo(info);
    DBUG_RETURN(arg_node);
}

node *CVIglobdecl(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIglobdecl");
    // DBUG_PRINT ("PST", ("PSTglobdecl"));
    DBUG_RETURN(arg_node);
}

node *CVIglobdef(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIglobdef");
    // DBUG_PRINT ("PST", ("PSTglobdef"));

    // // create the entry
    // node *entry = TBmakeSymboltableentry(GLOBDEF_NAME ( arg_node), GLOBDEF_TYPE ( arg_node), 0, arg_node, NULL, NULL);

    // // add to the current scope
    // if (!STadd(INFO_SYMBOL_TABLE ( arg_info), entry))
    // {
    //     DBUG_ASSERT (0, "Variable already declared");
    // }

    DBUG_RETURN(arg_node);
}

node *CVIstmts(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIstmts");
    // DBUG_PRINT ("PST", ("PSTstmts"));

    // // traverse over the sons
    // TRAVopt ( STMTS_STMT(arg_node), arg_info);
    // TRAVopt ( STMTS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *CVIvardecl(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIvardecl");

    node *expr = VARDECL_INIT(arg_node);
    VARDECL_INIT(arg_node) = NULL;
    node *varlet = TBmakeVarlet(STRcpy(VARDECL_NAME(arg_node)), NULL, NULL);
    // node *stmts = INFO_STMTS(arg_info);
    node *assign = TBmakeAssign(varlet, expr);

    if(INFO_LAST(arg_info) == NULL){
        INFO_STMTS(arg_info) = assign;
        INFO_LAST(arg_info) = assign;
    }else{
        STMTS_NEXT(INFO_LAST(arg_info)) = assign;
        INFO_LAST(arg_info) = assign;
    };

    TRAVopt(VARDECL_NEXT(arg_node), arg_info);

    // INFO_INITS ( arg_info) = init;

    DBUG_RETURN(arg_node);
}

/*
 * Traversal start function
 */
node *CVIdoChangeVariableInitialisations(node *syntaxtree)
{
    DBUG_ENTER("CVIdoChangeVariableInitialisations");
    // DBUG_PRINT ("CVI", ("CVIdoChangeVariableInitialisations"));

    info *info = MakeInfo();

    TRAVpush(TR_cvi);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    // STtraverse( INFO_SYMBOL_TABLE(info), printSymbolTable);

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
