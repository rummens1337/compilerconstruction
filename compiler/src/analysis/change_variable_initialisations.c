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
    node *front;
    node *last;
};

/*
 * INFO macros
 */

#define INFO_FRONT(n) ((n)->front)
#define INFO_LAST(n) ((n)->last)

// static void freeNode(node *node)
// {
//     nodetype type = NODE_TYPE (node);

//     if (type == N_binop) FREEbinop(node, NULL);
//     else if (type == N_monop) FREEmonop(node, NULL);
//     else if (type == N_funcall) FREEfuncall(node, NULL);
//     else if (type == N_cast) FREEcast(node, NULL);
//     else if (type == N_var) FREEvar(node, NULL);
//     else if (type == N_num) FREEnum(node, NULL);
//     else if (type == N_float) FREEfloat(node, NULL);
//     else if (type == N_bool) FREEbool(node, NULL);
//     else if (type == N_arrexpr) FREEarrexpr(node, NULL);
// }

/*
 * INFO functions
 */

static info *MakeInfo(void)
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));

    INFO_FRONT(result) = NULL;
    INFO_LAST(result) = NULL;

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

    // create make info
    info *info = MakeInfo();

    // traverse over the decls
    TRAVopt ( PROGRAM_DECLS ( arg_node), info);

    FreeInfo(info);

    DBUG_RETURN(arg_node);
}

/**
 * Traverse over decls (globdefs) and separate declaration and initialisation.
 */
node *CVIdecls(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIdecls");
    DBUG_PRINT("CVI", ("CVIdecls"));

    TRAVopt(DECLS_DECL(arg_node), arg_info);
    TRAVopt(DECLS_NEXT(arg_node), arg_info);

    node *stmts = INFO_FRONT(arg_info);

    // When there are no more decls, create __init function with initialisations of global.
    if (DECLS_NEXT(arg_node) == NULL && stmts != NULL)
    {
        node *funbod = TBmakeFunbody(NULL, NULL, stmts);
        DECLS_NEXT(arg_node) = TBmakeFundef(T_void, STRcpy("__init"), funbod, NULL);
    }

    DBUG_RETURN(arg_node);
}

node *CVIglobdef(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CVIglobdef");
    DBUG_PRINT("CVI", ("CVIglobdef"));
    node *expr = GLOBDEF_INIT(arg_node);

    if (expr)
    {
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

    if (expr)
    {
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
