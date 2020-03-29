#include "gen_byte_code.h"
#include "linked_list.h"
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
#include "print.h"

// TODO parameters funcall
// TODO funcalls
// TODO localfundefs bestaan niet? (wordt enkel aangeroepen)
// TODO stmts handig?

/*
 * INFO structure
 */
struct INFO
{
    FILE *fptr;
    node *symbol_table;
    listnode *const_pool;
    listnode *export_pool;
    listnode *global_pool;
    int global_counter;     // global
    int load_counter;       // global
    int store_counter;      // function bound
};

#define INFO_FILE(n) ((n)->fptr)
#define INFO_SYMBOL_TABLE(n) ((n)->symbol_table)
#define INFO_CONST_POOL(n) ((n)->const_pool)
#define INFO_EXPORT_POOL(n) ((n)->export_pool)
#define INFO_GLOBAL_POOL(n) ((n)->global_pool)
#define INFO_GLOBAL_COUNTER(n) ((n)->global_counter)
#define INFO_LOAD_COUNTER(n) ((n)->load_counter)
#define INFO_STORE_COUNTER(n) ((n)->store_counter)

/*
 * INFO functions
 */

static info *MakeInfo()
{
    info *result;

    DBUG_ENTER("MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));
    INFO_FILE(result) = NULL;
    INFO_SYMBOL_TABLE(result) = NULL;
    INFO_CONST_POOL(result) = NULL;
    INFO_EXPORT_POOL(result) = NULL;
    INFO_GLOBAL_POOL(result) = NULL;
    INFO_GLOBAL_COUNTER(result) = 0;
    INFO_LOAD_COUNTER(result) = 0;
    INFO_STORE_COUNTER(result) = 0;

    DBUG_RETURN(result);
}

static info *FreeInfo(info *info)
{
    DBUG_ENTER("FreeInfo");

    info = MEMfree(info);

    DBUG_RETURN(info);
}

void addToConstPool(info *arg_info, const char *value, int counter)
{
    // is the linked list set?
    if (INFO_CONST_POOL(arg_info) == NULL)
        INFO_CONST_POOL(arg_info) = LLcreate(value, counter, NULL);

    // append the value
    else
        LLappend(INFO_CONST_POOL(arg_info), value, counter);
}

void addToExportPool(info *arg_info, const char *value)
{
    // is the linked list set?
    if (INFO_EXPORT_POOL(arg_info) == NULL)
        INFO_EXPORT_POOL(arg_info) = LLcreate(value, 0, NULL);

    // append the value
    else
        LLappend(INFO_EXPORT_POOL(arg_info), value, 0);
}

void addToGlobalPool(info *arg_info, const char *value)
{
    // is the linked list set?
    if (INFO_GLOBAL_POOL(arg_info) == NULL)
        INFO_GLOBAL_POOL(arg_info) = LLcreate(value, 0, NULL);

    // append the value
    else
        LLappend(INFO_GLOBAL_POOL(arg_info), value, 0);
}


/**
 * Print const_pool, export_pool and global_pool values to
 * global.outfile
 * 
 * @param info arg_info
 * @return void
 */
void printGlobals(info *arg_info)
{
    // the pools
    listnode *const_pool = INFO_CONST_POOL(arg_info);
    listnode *export_pool = INFO_EXPORT_POOL(arg_info);
    listnode *global_pool = INFO_GLOBAL_POOL(arg_info);
    FILE *fileptr = INFO_FILE(arg_info);

    fprintf(fileptr, "\n"); // separate from other commands

    // Print constant pool values
    while (const_pool != NULL)
    {
        fprintf(fileptr, ".const %s\n", const_pool->value);
        const_pool = const_pool->next;
    }

    // Print constant pool values
    while (export_pool != NULL)
    {
        fprintf(fileptr, ".exportfun %s\n", export_pool->value);
        export_pool = export_pool->next;
    }

    // Print constant pool values
    while (global_pool != NULL)
    {
        fprintf(fileptr, ".global %s\n", global_pool->value);
        global_pool = global_pool->next;
    }
}

node *GBCprogram(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCprogram");
    DBUG_PRINT("GBC", ("GBCprogram"));

    // link the symbol table
    INFO_SYMBOL_TABLE(arg_info) = PROGRAM_SYMBOLTABLE(arg_node);

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

    node *table = INFO_SYMBOL_TABLE(arg_info);

    switch (SYMBOLTABLE_RETURNTYPE(table)){
        case T_int:
            fprintf(INFO_FILE(arg_info), "\t%s\n", "ireturn");
            break;
        case T_float:
            fprintf(INFO_FILE(arg_info), "\t%s\n", "freturn");
            break;
        case T_bool:
            fprintf(INFO_FILE(arg_info), "\t%s\n", "breturn");
            break;
        case T_void:
            // TODO pass?
            break;
        case T_unknown:
            CTIabort("Unknown type found in file: %s, line: %s", __FILE__, __LINE__);
            break;
        }

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

    // print to the file
    fprintf(INFO_FILE(arg_info), "%s:\n", FUNDEF_NAME(arg_node));

    // is this an exported fundef?
    if (FUNDEF_ISEXPORT(arg_node)){
        // Create export pool string
        int length = snprintf(NULL, 0, "\"%s\" %s %s", FUNDEF_NAME(arg_node),
                stype(FUNDEF_TYPE(arg_node)), FUNDEF_NAME(arg_node));
        char* str = malloc( length + 1 );
        snprintf(str, length + 1, "\"%s\" %s %s", FUNDEF_NAME(arg_node), 
                stype(FUNDEF_TYPE(arg_node)), FUNDEF_NAME(arg_node));

        addToExportPool(arg_info, STRcpy(str));
    }

    // store the symbol table
    node *table = INFO_SYMBOL_TABLE(arg_info);

    // get the entry from the symbol table
    node *entry = STsearchFundef(table, FUNDEF_NAME(arg_node));

    // set the symbol table for the upcoming scope
    INFO_SYMBOL_TABLE(arg_info) = SYMBOLTABLEENTRY_TABLE(entry); // nested symbol table

    // traverse over the params and body
    TRAVopt(FUNDEF_PARAMS(arg_node), arg_info);
    TRAVopt(FUNDEF_FUNBODY(arg_node), arg_info);
    
    // revert the symbol table
    INFO_SYMBOL_TABLE(arg_info) = table; // global symbol table
    INFO_STORE_COUNTER(arg_info) = 0; // Reset scope counter for new function.

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

    switch (GLOBDEF_TYPE(arg_node)){
    case T_int:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "istoreg");
        addToGlobalPool(arg_info, "int");
        break;
    case T_float:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "fstoreg");
        addToGlobalPool(arg_info, "float");
        break;
    case T_bool:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "bstoreg");
        addToGlobalPool(arg_info, "bool");
        break;
    case T_void:
        // TODO: hoort void type hier eigenlijk wel bij?
        break;
    case T_unknown:
        CTIabort("Unknown type found in file: %s, line: %s", __FILE__, __LINE__);
        break;
    }

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

    TRAVopt(VARDECL_DIMS(arg_node), arg_info);
    TRAVopt(VARDECL_INIT(arg_node), arg_info);
    TRAVopt(VARDECL_NEXT(arg_node), arg_info);

    switch (VARDECL_TYPE(arg_node)){
    case T_int:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "istore");
        break;
    case T_float:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "fstore");
        break;
    case T_bool:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "bstore");
        break;
    case T_void:
        // TODO: hoort void type hier eigenlijk wel bij?
        break;
    case T_unknown:
        CTIabort("Unknown type found in file: %s, line: %s", __FILE__, __LINE__);
        break;
    }

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
    //    fprintf(INFO_FILE(arg_info), "\t%d\n", __LINE__);

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

    switch (BINOP_OP(arg_node))
    {
    case BO_add:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "iadd");
        fprintf(INFO_FILE(arg_info), "\t%s\n", "fadd");
        fprintf(INFO_FILE(arg_info), "\t%s\n", "badd");
        break;
    case BO_sub:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "isub");
        break;
    case BO_mul:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "imul");
        break;
    case BO_div:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "idiv");
        break;
    case BO_mod:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_lt:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_le:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_gt:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_ge:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_eq:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_ne:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_and:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_or:
        fprintf(INFO_FILE(arg_info), "\t%s\n", "irem"); // Klopt deze?
        break;
    case BO_unknown:
        CTIabort("Unknown operator type found in file: %s, line: %s", __FILE__, __LINE__);
        break;
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

    // Create const pool byte code string
    int length = snprintf(NULL, 0, "int %d", NUM_VALUE(arg_node));
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "int %d", NUM_VALUE(arg_node));

    // Search linked list for const value
    listnode *const_pool = LLsearch(INFO_CONST_POOL(arg_info), str);

    // Add to const pool if it doesn't exist yet.
    // Else extract values from linked list and print to file.
    if(const_pool == NULL){
        addToConstPool(arg_info, str, INFO_LOAD_COUNTER(arg_info));
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "iloadc", INFO_LOAD_COUNTER(arg_info));
        INFO_LOAD_COUNTER(arg_info) +=1;
    }else{
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "iloadc", const_pool->counter);
    }

    DBUG_RETURN(arg_node);
}

node *GBCfloat(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfloat");
    DBUG_PRINT("GBC", ("GBCfloat"));

    // Create const pool byte code string
    int length = snprintf(NULL, 0, "float %f", FLOAT_VALUE(arg_node));
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "float %f", FLOAT_VALUE(arg_node));

    // Search linked list for const value
    listnode *const_pool = LLsearch(INFO_CONST_POOL(arg_info), str);

    // Add to const pool if it doesn't exist yet.
    // Else extract values from linked list and print to file.
    if(const_pool == NULL){
        addToConstPool(arg_info, str, INFO_LOAD_COUNTER(arg_info));
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "floadc", INFO_LOAD_COUNTER(arg_info));
        INFO_LOAD_COUNTER(arg_info) +=1;
    }else{
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "floadc", const_pool->counter);
    }

    DBUG_RETURN(arg_node);
}

node *GBCbool(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCbool");
    DBUG_PRINT("GBC", ("GBCbool"));

    // Create const pool byte code string
    int length = snprintf(NULL, 0, "bool %s", BOOL_VALUE(arg_node) ? "true" : "false");
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "bool %s", BOOL_VALUE(arg_node) ? "true" : "false");

    // Search linked list for const value
    listnode *const_pool = LLsearch(INFO_CONST_POOL(arg_info), str);

    // Add to const pool if it doesn't exist yet.
    // Else extract values from linked list and print to file.
    if(const_pool == NULL){
        addToConstPool(arg_info, str, INFO_LOAD_COUNTER(arg_info));
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "floadc", INFO_LOAD_COUNTER(arg_info));
        INFO_LOAD_COUNTER(arg_info) +=1;
    }else{
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "floadc", const_pool->counter);
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

    info *info = MakeInfo();

    INFO_FILE(info) = fopen("src/codegen/global.outfile", "w");

    if (INFO_FILE(info) == NULL)
        CTIabort("Could not open file: %s", "src/codegen/global.outfile");

    TRAVpush(TR_gbc);
    syntaxtree = TRAVdo(syntaxtree, info);
    TRAVpop();

    printGlobals(info);

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
