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


// TODO T_void als return type werkt niet.
// TODO monop neg en not nalopen, kijken wat het verschil qua implementatie is.

/*
 * INFO structure
 */
struct INFO
{
    FILE *fptr;
    node *symbol_table;
    node *symbol_table_entry;

    listnode *const_pool;
    listnode *export_pool;
    listnode *global_pool;
    int global_counter;  // counts amound of globals {0..n}
    int load_counter;    // counts amound of loads {0..n}
    int branch_count;    // counts amound of stores - function bound {0..n}
    int current_type;    // Current type of var {int, float, bool}
};

#define INFO_FILE(n) ((n)->fptr)
#define INFO_SYMBOL_TABLE(n) ((n)->symbol_table)
#define INFO_SYMBOL_TABLE_ENTRY(n) ((n)->symbol_table_entry)

#define INFO_CONST_POOL(n) ((n)->const_pool)
#define INFO_EXPORT_POOL(n) ((n)->export_pool)
#define INFO_GLOBAL_POOL(n) ((n)->global_pool)
#define INFO_GLOBAL_COUNTER(n) ((n)->global_counter)
#define INFO_LOAD_COUNTER(n) ((n)->load_counter)
#define INFO_BRANCH_COUNT(n) ((n)->branch_count)
#define INFO_CURRENT_TYPE(n) ((n)->current_type)

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
    INFO_SYMBOL_TABLE_ENTRY(result) = NULL;
    INFO_CONST_POOL(result) = NULL;
    INFO_EXPORT_POOL(result) = NULL;
    INFO_GLOBAL_POOL(result) = NULL;
    INFO_GLOBAL_COUNTER(result) = 0;
    INFO_LOAD_COUNTER(result) = 0;
    INFO_BRANCH_COUNT(result) = 0;
    INFO_CURRENT_TYPE(result) = T_unknown; // current const type

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
void writeGlobals(info *arg_info)
{
    // the pools
    listnode *const_pool = INFO_CONST_POOL(arg_info);
    listnode *export_pool = INFO_EXPORT_POOL(arg_info);
    listnode *global_pool = INFO_GLOBAL_POOL(arg_info);
    FILE *fileptr = INFO_FILE(arg_info);

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

    TRAVopt(RETURN_EXPR(arg_node), arg_info);

    switch (SYMBOLTABLE_RETURNTYPE(table))
    {
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
        fprintf(INFO_FILE(arg_info), "\t%s\n", "return");
        break;
    case T_unknown:
        CTIabort("Unknown type found in file: %s, line: %s", __FILE__, __LINE__);
        break;
    }

    DBUG_RETURN(arg_node);
}

node *GBCfuncall(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfuncall");
    DBUG_PRINT("GBC", ("GBCfuncall"));

    node *entry = STdeepSearchFundef(INFO_SYMBOL_TABLE ( arg_info), FUNCALL_NAME ( arg_node));

    switch (SYMBOLTABLEENTRY_TYPE ( entry))
    {
    case T_int:   fprintf(INFO_FILE ( arg_info), "\tisrg\n"); break;
    case T_float: fprintf(INFO_FILE ( arg_info), "\tfsrg\n"); break;
    case T_bool:  fprintf(INFO_FILE ( arg_info), "\tbsrg\n"); break;
    case T_void:  break;
    case T_unknown: break;
    }

    TRAVopt(FUNCALL_ARGS(arg_node), arg_info);

    // print
    node *table = SYMBOLTABLEENTRY_TABLE ( entry);
    fprintf(INFO_FILE ( arg_info), "\tjsr %ld %s\n", STparams ( table), FUNCALL_NAME ( arg_node));

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

    // store the symbol table
    node *table = INFO_SYMBOL_TABLE(arg_info);

    // get the entry from the symbol table
    node *entry = STsearchFundef(table, FUNDEF_NAME(arg_node));

    // is this an exported fundef?
    if (FUNDEF_ISEXPORT(arg_node))
    {
        // get the entry
        node *fentry = SYMBOLTABLE_ENTRY ( SYMBOLTABLEENTRY_TABLE ( entry));

        char *params = NULL;

        // loop over the entries
        for (; fentry != NULL; fentry = SYMBOLTABLEENTRY_NEXT ( fentry))
        {
            // do we have a param entry
            if (!SYMBOLTABLEENTRY_PARAM ( fentry)) continue;

           char* temp = STRcatn(3, params, " ", stype ( SYMBOLTABLEENTRY_TYPE ( fentry)));
           free(params);

           params = temp;
        }

        // Create export pool string
        int length = snprintf(
            NULL,
            0,
            "\"%s\" %s %s %s",
            FUNDEF_NAME(arg_node),
            stype(FUNDEF_TYPE(arg_node)),
            params == NULL ? "" : params,
            FUNDEF_NAME(arg_node)
        );

        char *str = (char *)malloc(length + 1);

        snprintf(
            str,
            length + 1,
            "\"%s\" %s %s %s",
            FUNDEF_NAME(arg_node),
            stype(FUNDEF_TYPE(arg_node)),
            params == NULL ? "" : params,
            FUNDEF_NAME(arg_node)
        );

        addToExportPool(arg_info, str);
    }

    // set the symbol table for the upcoming scope
    INFO_SYMBOL_TABLE(arg_info) = SYMBOLTABLEENTRY_TABLE(entry); // nested symbol table

    // number of registers to use
    size_t registers = STcount( INFO_SYMBOL_TABLE ( arg_info));

    if (registers > 0)
    {
        // Print amount of function vardecls
        fprintf(
            INFO_FILE(arg_info),
            "\tesr %ld\n",
            registers > 4 ? 4 : registers
        );
    }

    // traverse over the params and body
    TRAVopt(FUNDEF_PARAMS(arg_node), arg_info);
    TRAVopt(FUNDEF_FUNBODY(arg_node), arg_info);

    // If return type is void, add 'return' as last instruction to function.
    if (FUNDEF_TYPE(arg_node) == T_void)
        fprintf(INFO_FILE(arg_info), "\t%s\n", "return");

    // reset the symbol table
    INFO_SYMBOL_TABLE(arg_info) = table;

    // print end of line
    fputc('\n', INFO_FILE(arg_info));

    DBUG_RETURN(arg_node);
}

node *GBCfunbody(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfunbody");
    DBUG_PRINT("GBC", ("GBCfunbody"));

    // iterate over the nodes
    TRAVopt(FUNBODY_VARDECLS(arg_node), arg_info);
    TRAVopt(FUNBODY_STMTS(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCifelse(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCifelse");
    DBUG_PRINT("GBC", ("GBCifelse"));

    TRAVdo(IFELSE_COND(arg_node), arg_info);

    INFO_BRANCH_COUNT ( arg_info) += 1;

    char *branch = STRcatn(
        3, 
        STRitoa(INFO_BRANCH_COUNT ( arg_info)),
        "_",
        IFELSE_ELSE ( arg_node) == NULL ? "if" : "else"
    );

    fprintf ( 
        INFO_FILE ( arg_info),
        "\tbranch_f %s\n\n",
        branch
    );

    INFO_BRANCH_COUNT ( arg_info) += 1;
    char *end = STRcat( 
        STRitoa(INFO_BRANCH_COUNT ( arg_info)),
        "_end"
    );

    TRAVopt(IFELSE_THEN(arg_node), arg_info);

    if (IFELSE_ELSE(arg_node) != NULL)
    {
        INFO_BRANCH_COUNT ( arg_info) += 1;
        fprintf ( INFO_FILE ( arg_info), "\tjump %s\n\n", end);

        fprintf ( INFO_FILE ( arg_info), "%s:\n", branch);
        TRAVopt(IFELSE_ELSE(arg_node), arg_info);
        fputc ('\n', INFO_FILE ( arg_info));
    }
    
    fprintf ( INFO_FILE ( arg_info), "%s:\n", end);

    // free the string
    free(branch);
    free(end);

    DBUG_RETURN(arg_node);
}

node *GBCwhile(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCwhile");
    DBUG_PRINT("GBC", ("GBCwhile"));

    // fprintf(INFO_FILE(arg_info), "\t%s\n", "BEGIN_WHILE");

    TRAVdo(WHILE_COND(arg_node), arg_info);
    TRAVopt(WHILE_BLOCK(arg_node), arg_info);
    // fprintf(INFO_FILE(arg_info), "\t%s\n", "E_WHILE");

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

    switch (GLOBDEF_TYPE(arg_node))
    {
    case T_int:
        addToGlobalPool(arg_info, "int");
        break;
    case T_float:
        addToGlobalPool(arg_info, "float");
        break;
    case T_bool:
        addToGlobalPool(arg_info, "bool");
        break;
    case T_void:
        // TODO: hoort void type hier eigenlijk wel bij?
        break;
    case T_unknown:
        CTIabort("Unknown type found in file: %s, line: %s", __FILE__, __LINE__);
        break;
    }

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

    TRAVopt(VARDECL_DIMS(arg_node), arg_info);
    TRAVopt(VARDECL_INIT(arg_node), arg_info);

    // fprintf(INFO_FILE(arg_info), "\t%s\n", "VARDECL");

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

    node *entry = INFO_SYMBOL_TABLE_ENTRY(arg_info);

    // store count
    char type;
    switch (SYMBOLTABLEENTRY_TYPE ( entry))
    {
    case T_int: type = 'i'; break;
    case T_float: type = 'f'; break;
    case T_bool: type = 'b'; break;
    case T_void:
    case T_unknown: break;
    }

    if (SYMBOLTABLEENTRY_DEPTH ( entry) == 0) fprintf(INFO_FILE(arg_info), "\t%cstoreg %d\n", type, SYMBOLTABLEENTRY_OFFSET ( entry));
    else fprintf ( INFO_FILE ( arg_info), "\t%cstore %d\n", type, SYMBOLTABLEENTRY_OFFSET ( entry));

    // increment the store
    INFO_SYMBOL_TABLE_ENTRY(arg_info) = NULL;

    DBUG_RETURN(arg_node);
}

node *GBCvarlet(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCvarlet");
    DBUG_PRINT("GBC", ("GBCvarlet"));

    // set the current 
    node *table = INFO_SYMBOL_TABLE ( arg_info);
    INFO_SYMBOL_TABLE_ENTRY(arg_info) = STdeepSearchVariableByName ( table, VARLET_NAME ( arg_node));

    DBUG_RETURN(arg_node);
}

node *GBCbinop(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCbinop");
    DBUG_PRINT("GBC", ("GBCbinop"));

    TRAVdo(BINOP_LEFT(arg_node), arg_info);
    TRAVdo(BINOP_RIGHT(arg_node), arg_info);

    const char *operation;
    switch (BINOP_OP(arg_node))
    {
    case BO_add: operation = "add"; break;
    case BO_sub: operation = "sub"; break;
    case BO_mul: operation = "mul"; break;
    case BO_div: operation = "div"; break;
    case BO_mod: operation = "rem"; break;
    case BO_lt:  operation = "lt"; break;
    case BO_le:  operation = "le"; break;
    case BO_gt:  operation = "gt"; break;
    case BO_ge:  operation = "ge"; break;
    case BO_eq:  operation = "eq"; break;
    case BO_ne:  operation = "ne"; break;

    // TODO: these are replaced with ternary statements
    case BO_and:
    case BO_or:
    case BO_unknown:
        CTIabort("Unknown operator type found in file: %s, line: %s", __FILE__, __LINE__);
        break;
    }

    node *entry = INFO_SYMBOL_TABLE_ENTRY(arg_info);
    switch (SYMBOLTABLEENTRY_TYPE ( entry))
    {
    case T_int:   fprintf(INFO_FILE(arg_info), "\ti%s\n", operation); break;
    case T_float: fprintf(INFO_FILE(arg_info), "\tf%s\n", operation); break;
    case T_bool:  fprintf(INFO_FILE(arg_info), "\tb%s\n", operation); break;
    case T_void:
    case T_unknown: break;
    }

    DBUG_RETURN(arg_node);
}

node *GBCmonop(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCmonop");
    DBUG_PRINT("GBC", ("GBCmonop"));

    TRAVdo(MONOP_OPERAND(arg_node), arg_info);

    switch (MONOP_OP(arg_node))
    {
    case MO_minus:
        switch (INFO_CURRENT_TYPE(arg_info))
        {
        case T_int:
            fprintf(INFO_FILE(arg_info), "\t%s\n", "ineg");
            break;
        case T_float:
            fprintf(INFO_FILE(arg_info), "\t%s\n", "fneg");
            break;
        }
        break;
    case MO_neg:
        switch (INFO_CURRENT_TYPE(arg_info))
        {
        case T_int:
            fprintf(INFO_FILE(arg_info), "\t%s\n", "ineg");
            break;
        case T_float:
            fprintf(INFO_FILE(arg_info), "\t%s\n", "fneg");
            break;
        }
        break;
    case MO_unknown:
        CTIabort("Unknown operator type found in file: %s, line: %s", __FILE__, __LINE__);
        break;
    }

    DBUG_RETURN(arg_node);
}

node *GBCvar(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCvar");
    DBUG_PRINT("GBC", ("GBCvar"));

    node *entry = STdeepSearchVariableByName ( INFO_SYMBOL_TABLE ( arg_info), VAR_NAME ( arg_node));
    
    // is this the global scope?
    if ( SYMBOLTABLEENTRY_DEPTH ( entry) == 0)
    {
        if ( SYMBOLTABLEENTRY_TYPE ( entry) == T_int) fprintf(INFO_FILE(arg_info), "\tiloadg %d\n", SYMBOLTABLEENTRY_OFFSET ( entry));
        else if (SYMBOLTABLEENTRY_TYPE ( entry) == T_float) fprintf(INFO_FILE(arg_info), "\tfloadg %d\n", SYMBOLTABLEENTRY_OFFSET ( entry));
    }

    // 
    else
    {
        if ( SYMBOLTABLEENTRY_TYPE ( entry) == T_int) fprintf(
            INFO_FILE(arg_info), 
            SYMBOLTABLEENTRY_PARAM ( entry) ? "\tiload_%d\n" : "\tiload %d\n",
            SYMBOLTABLEENTRY_OFFSET ( entry)
        );
        else if (SYMBOLTABLEENTRY_TYPE ( entry) == T_float) fprintf(
            INFO_FILE(arg_info),
            SYMBOLTABLEENTRY_PARAM ( entry) ? "\tfload_%d\n" : "\tfload %d\n",
            SYMBOLTABLEENTRY_OFFSET ( entry)
        );
    }

    DBUG_RETURN(arg_node);
}

node *GBCnum(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCnum");
    DBUG_PRINT("GBC", ("GBCnum"));

    // Create const pool byte code string
    int length = snprintf(NULL, 0, "int %d", NUM_VALUE(arg_node));
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "int %d", NUM_VALUE(arg_node));

    // Search linked list for const value
    listnode *const_pool = LLsearch(INFO_CONST_POOL(arg_info), str);

    // Add to const pool if it doesn't exist yet.
    // Else extract values from linked list and print to file.
    if (const_pool == NULL)
    {
        addToConstPool(arg_info, STRcpy(str), INFO_LOAD_COUNTER(arg_info));
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "iloadc", INFO_LOAD_COUNTER(arg_info));
        INFO_LOAD_COUNTER(arg_info) += 1;
    }
    else
    {
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "iloadc", const_pool->counter);
    }

    // Set current type to int
    INFO_CURRENT_TYPE(arg_info) = T_int;

    DBUG_RETURN(arg_node);
}

node *GBCfloat(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCfloat");
    DBUG_PRINT("GBC", ("GBCfloat"));

    // Create const pool byte code string
    int length = snprintf(NULL, 0, "float %f", FLOAT_VALUE(arg_node));
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "float %f", FLOAT_VALUE(arg_node));

    // Search linked list for const value
    listnode *const_pool = LLsearch(INFO_CONST_POOL(arg_info), str);

    // Add to const pool if it doesn't exist yet.
    // Else extract values from linked list and print to file.
    if (const_pool == NULL)
    {
        addToConstPool(arg_info, STRcpy(str), INFO_LOAD_COUNTER(arg_info));
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "floadc", INFO_LOAD_COUNTER(arg_info));
        INFO_LOAD_COUNTER(arg_info) += 1;
    }
    else
    {
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "floadc", const_pool->counter);
    }

    // Set current type to float
    INFO_CURRENT_TYPE(arg_info) = T_float;

    DBUG_RETURN(arg_node);
}

node *GBCbool(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCbool");
    DBUG_PRINT("GBC", ("GBCbool"));

    // Create const pool byte code string
    int length = snprintf(NULL, 0, "bool %s", BOOL_VALUE(arg_node) ? "true" : "false");
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "bool %s", BOOL_VALUE(arg_node) ? "true" : "false");

    // Search linked list for const value
    listnode *const_pool = LLsearch(INFO_CONST_POOL(arg_info), str);

    // Add to const pool if it doesn't exist yet.
    // Else extract values from linked list and print to file.
    if (const_pool == NULL)
    {
        addToConstPool(arg_info, STRcpy(str), INFO_LOAD_COUNTER(arg_info));
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "bloadc", INFO_LOAD_COUNTER(arg_info));
        INFO_LOAD_COUNTER(arg_info) += 1;
    }
    else
    {
        fprintf(INFO_FILE(arg_info), "\t%s %d\n", "bloadc", const_pool->counter);
    }

    // Set current type to bool
    INFO_CURRENT_TYPE(arg_info) = T_bool;

    DBUG_RETURN(arg_node);
}

node *GBCerror(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCerror");
    DBUG_PRINT("GBC", ("GBCerror"));

    TRAVopt(ERROR_NEXT(arg_node), arg_info);

    DBUG_RETURN(arg_node);
}

node *GBCternary(node *arg_node, info *arg_info)
{
    DBUG_ENTER("GBCternary");
    DBUG_PRINT("GBC", ("GBCternary"));

    TRAVopt(TERNARY_EXPR(arg_node), arg_info);
    TRAVopt(TERNARY_FALSE(arg_node), arg_info);
    TRAVopt(TERNARY_TRUE(arg_node), arg_info);

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

    writeGlobals(info);

    // free the pointer
    FreeInfo(info);

    DBUG_RETURN(syntaxtree);
}
