#include "gen_byte_code.h"
#include "symbol_table.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "ctinfo.h"
#include <stdio.h>
#include "memory.h"
#include "free.h"
#include "str.h"


// Global file pointer, used by many nodes.
FILE * fptr;
node * symbol_table; // TODO remove?

struct INFO{
    const char *byte_string;
    info * next;
};

#define INFO_BYTE_STRING(n) ((n)->byte_string)
#define INFO_NEXT(n) ((n)->next)

/*
 * INFO functions
 */

static info *MakeInfo(){
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));
    INFO_BYTE_STRING ( result) = NULL;
    INFO_NEXT ( result) = NULL;

    DBUG_RETURN(result);
}

static info *FreeInfo(info *info)
{
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

// /**
//  * SYMTAB functions
//  */
// typedef struct SYMTAB symtab;
// struct SYMTAB{
//     node * table;
// };

// #define SYMTAB_TABLE(n) ((n)->table)

// static symtab *MakeSymtab(){
//     symtab *result;

//     DBUG_ENTER("MakeSymtab");

//     result = (symtab *)MEMmalloc(sizeof(symtab));
//     SYMTAB_TABLE ( result) = NULL;

//     DBUG_RETURN(result);
// }

void addToConstPool(info * arg_info, const char * byte_string){
    /* 2. allocate new node */
    info * next = MakeInfo();
    /* 3. put new data in node */
    INFO_BYTE_STRING(next) = byte_string;
    /* 4. Make next of new node as next of prev_node */
    INFO_NEXT(next) = INFO_NEXT(arg_info);
    /* 5. move the next of prev_node as new_node */
    INFO_NEXT(arg_info) = next;
}

/* Function to reverse the linked list */
static void reverse(info** head_ref) 
{ 
    info* prev = NULL; 
    info* current = *head_ref; 
    info* next = NULL; 
    while (current != NULL) { 
        // Store next 
        next = current->next; 
  
        // Reverse current node's pointer 
        INFO_NEXT(current) = prev; 
  
        // Move pointers one position ahead. 
        prev = current; 
        current = next; 
    } 
    *head_ref = prev; 
} 

void printGlobals(info * node){
    reverse(&node);

    while (node != NULL) {
        if(INFO_BYTE_STRING(node) != NULL)
            fprintf(fptr, "\t%s\n", INFO_BYTE_STRING(node));
        node = INFO_NEXT(node); 
    }
}

node *GBCprogram(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCprogram");
    DBUG_PRINT("GBC", ("GBCprogram"));

    // link the symbol table
    symbol_table = PROGRAM_SYMBOLTABLE ( arg_node);

    TRAVdo(PROGRAM_DECLS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCsymboltable(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCsymboltable");
    DBUG_PRINT("GBC", ("GBCsymboltable"));

    DBUG_RETURN(arg_node);
}

node *GBCsymboltableentry(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCsymboltableentry");
    DBUG_PRINT("GBC", ("GBCsymboltableentry"));

    DBUG_RETURN(arg_node);
}

node *GBCdecls(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCdecls");
    DBUG_PRINT("GBC", ("GBCdecls"));

    TRAVdo(DECLS_DECL(arg_node), arg_info);
    TRAVopt(DECLS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCexprs(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCexprs");
    DBUG_PRINT("GBC", ("GBCexprs"));

    TRAVdo(EXPRS_EXPR(arg_node), arg_info);
    TRAVopt(EXPRS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCarrexpr(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCarrexpr");
    DBUG_PRINT("GBC", ("GBCarrexpr"));

    DBUG_RETURN(arg_node);
}

node *GBCids(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCids");
    DBUG_PRINT("GBC", ("GBCids"));

    TRAVopt(IDS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCexprstmt(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCexprstmt");
    DBUG_PRINT("GBC", ("GBCexprstmt"));

    TRAVdo(EXPRSTMT_EXPR(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCreturn(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCreturn");
    DBUG_PRINT("GBC", ("GBCreturn"));

    fprintf(fptr, "\t%s\n", "?return");

    TRAVopt(RETURN_EXPR(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCfuncall(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfuncall");
    DBUG_PRINT("GBC", ("GBCfuncall"));

    TRAVopt(FUNCALL_ARGS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCcast(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCcast");
    DBUG_PRINT("GBC", ("GBCcast"));

    TRAVopt(CAST_EXPR(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCfundefs(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfundefs");
    DBUG_PRINT("GBC", ("GBCfundefs"));

    TRAVdo(FUNDEFS_FUNDEF(arg_node), arg_info);
    TRAVopt(FUNDEFS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCfundef(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfundef");
    DBUG_PRINT("GBC", ("GBCfundef"));

    fprintf(fptr, "%s:\n", FUNDEF_NAME(arg_node));
    
    if(FUNDEF_ISEXPORT(arg_node))
        fprintf(fptr, "%s:\n", FUNDEF_NAME(arg_node));

    TRAVopt(FUNDEF_FUNBODY(arg_node), arg_info);
    TRAVopt(FUNDEF_PARAMS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCfunbody(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfunbody");
    DBUG_PRINT("GBC", ("GBCfunbody"));

    TRAVopt(FUNBODY_VARDECLS(arg_node), arg_info);
    TRAVopt(FUNBODY_LOCALFUNDEFS(arg_node), arg_info);
    TRAVopt(FUNBODY_STMTS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCifelse(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCifelse");
    DBUG_PRINT("GBC", ("GBCifelse"));

    TRAVdo(IFELSE_COND(arg_node), arg_info);
    TRAVopt(IFELSE_THEN(arg_node), arg_info);
    TRAVopt(IFELSE_ELSE(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCwhile(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCwhile");
    DBUG_PRINT("GBC", ("GBCwhile"));

    TRAVdo(WHILE_COND(arg_node), arg_info);
    TRAVopt(WHILE_BLOCK(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCdowhile(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCdowhile");
    DBUG_PRINT("GBC", ("GBCdowhile"));

    TRAVdo(DOWHILE_COND(arg_node), arg_info);
    TRAVopt(DOWHILE_BLOCK(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCfor(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfor");
    DBUG_PRINT("GBC", ("GBCfor"));

    TRAVdo(FOR_START(arg_node), arg_info);
    TRAVdo(FOR_STOP(arg_node), arg_info);
    TRAVopt(FOR_STEP(arg_node), arg_info);
    TRAVopt(FOR_BLOCK(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCglobdecl(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCglobdecl");
    DBUG_PRINT("GBC", ("GBCglobdecl"));

    TRAVopt(GLOBDECL_DIMS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCglobdef(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCglobdef");
    DBUG_PRINT("GBC", ("GBCglobdef"));

    TRAVopt(GLOBDEF_DIMS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCparam(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCparam");
    DBUG_PRINT("GBC", ("GBCparam"));

    TRAVopt(PARAM_DIMS(arg_node), arg_info);
    TRAVopt(PARAM_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCvardecl(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCvardecl");
    DBUG_PRINT("GBC", ("GBCvardecl"));
    
    addToConstPool(arg_info, VARDECL_NAME(arg_node));

    TRAVopt(VARDECL_DIMS(arg_node), arg_info);
    TRAVopt(VARDECL_INIT(arg_node), arg_info);
    TRAVopt(VARDECL_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCstmts(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCstmts");
    DBUG_PRINT("GBC", ("GBCstmts"));

    TRAVdo(STMTS_STMT(arg_node), arg_info);
    TRAVopt(STMTS_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCassign(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCassign");
    DBUG_PRINT("GBC", ("GBCassign"));

    TRAVopt(ASSIGN_LET(arg_node), arg_info);
    TRAVdo(ASSIGN_EXPR(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCbinop(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCbinop");
    DBUG_PRINT("GBC", ("GBCbinop"));


    TRAVdo(BINOP_LEFT(arg_node), arg_info);
    TRAVdo(BINOP_RIGHT(arg_node), arg_info);

    switch (BINOP_OP(arg_node)){
        case BO_add:
            fprintf(fptr, "\t%s\n", "iadd");
            break;
        case BO_sub:
            fprintf(fptr, "\t%s\n", "isub");
            break;
        case BO_mul:
            fprintf(fptr, "\t%s\n", "imul");
            break;
        case BO_div:
            fprintf(fptr, "\t%s\n", "idiv");
            break;
        case BO_mod:
            fprintf(fptr, "\t%s\n", "irem"); // Klopt deze?
            break;
        default:
            "nope";
    }
    
    DBUG_RETURN(arg_node);
}

node *GBCmonop(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCmonop");
    DBUG_PRINT("GBC", ("GBCmonop"));

    TRAVdo(MONOP_OPERAND(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCvarlet(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCvarlet");
    DBUG_PRINT("GBC", ("GBCvarlet"));

    TRAVopt(VARLET_INDICES(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCvar(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCvar");
    DBUG_PRINT("GBC", ("GBCvar"));

    TRAVopt(VAR_INDICES(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCnum(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCnum");
    DBUG_PRINT("GBC", ("GBCnum"));

    fprintf(fptr, "\t%s %d\n", "iloadc", 0);

    DBUG_RETURN(arg_node);
}

node *GBCfloat(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfloat");
    DBUG_PRINT("GBC", ("GBCfloat"));

    fprintf(fptr, "\t%s %d\n", "floadc", 0);

    DBUG_RETURN(arg_node);
}

node *GBCbool(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCbool");
    DBUG_PRINT("GBC", ("GBCbool"));

    switch(BOOL_VALUE(arg_node)){
        case TRUE:
            fprintf(fptr, "\t%s\n", "bload_t");
            break;
        case FALSE:
            fprintf(fptr, "\t%s\n", "bload_f");
        break;
    }

    DBUG_RETURN(arg_node);
}

node *GBCerror(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCerror");
    DBUG_PRINT("GBC", ("GBCerror"));

    TRAVopt(ERROR_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

/*
 * Traversal start function
 */

node *GBCdoGenByteCode(node *syntaxtree)
{
    DBUG_ENTER("GBCdoGenByteCode");
    DBUG_PRINT("GBC", ("GBCdoGenByteCode"));

    fptr = fopen("src/codegen/global.outfile", "w");

    if (fptr == NULL)
    {
        CTIerror ("Could not open file: %s", "src/codegen/global.outfile");
    }

    info *info = MakeInfo();

    TRAVpush(TR_gbc);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    printGlobals(info);

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
