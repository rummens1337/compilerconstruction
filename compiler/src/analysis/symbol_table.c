
#include "string.h"
#include "symbol_table.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "memory.h"
#include "free.h"
#include "str.h"
#include "ctinfo.h"

/**
 *  Forward decleration
 */
static node *STgetEntry(node *list, int offset);
static node *STsearchVariableEntry(node *list, const char *name, type type);
static node *STsearchFundefEntry(node *list, const char *name);
static node *STlastEntry(node *list);
static void STtraverseEntries(node *list, void callback( void *));
static void STdisplayEntry(node *list, size_t tabs);

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  offset      the offset
 *  @return node|NULL
 */
node *STget(node *table, int offset)
{
    // the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // return the result
    return STgetEntry( entry, offset);
}

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  list        the symbol table entry node
 *  @param  offset      the offset
 *  @return node|NULL
 */
node *STgetEntry(node *list, int offset)
{
    // do we have a valid entry
    if ( list == NULL) return NULL;

    // check if the name is the same
    if ( SYMBOLTABLEENTRY_OFFSET (list) != offset) return STgetEntry(SYMBOLTABLEENTRY_NEXT (list), offset);

    // return the result
    return list;
}

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearchVariable(node *table, const char *name, type type)
{
    // the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // return the result
    return STsearchVariableEntry( entry, name, type);
}

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  list        the symbol table entry node
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearchVariableEntry(node *list, const char *name, type type)
{
    // do we have a valid entry
    if ( list == NULL) return NULL;

    // skip fundefs
    if ( SYMBOLTABLEENTRY_TABLE ( list) != NULL) return STsearchVariableEntry(SYMBOLTABLEENTRY_NEXT (list), name, type);

    // check if the name is the same
    if ( strcmp(SYMBOLTABLEENTRY_NAME ( list), name) != 0) return STsearchVariableEntry(SYMBOLTABLEENTRY_NEXT (list), name, type);

    // check if the type is the same
    if ( type != T_unknown && SYMBOLTABLEENTRY_TYPE ( list) != type) return STsearchVariableEntry(SYMBOLTABLEENTRY_NEXT (list), name, type);

    // return the result
    return list;
}

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearchVariableByName(node *table, const char *name)
{
    // the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // return the result
    return STsearchVariableEntry( entry, name, T_unknown);
}

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @return node|NULL
 */
node *STdeepSearchVariableByName(node *table, const char *name)
{
    // search for the node in the current scope
    node *found = STsearchVariableByName( table, name);

    // do we have a node?
    if (found != NULL) return found;

    // get the parent table
    node *parent = SYMBOLTABLE_PARENT ( table);

    // do we have a parent table?
    if (parent == NULL) return NULL;

    // search for the node in the parent table
    return STdeepSearchVariableByName(parent, name);
}


/**
 *  Find an entry by its link
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @return node|NULL
 */
node *STdeepSearchByNode(node *table, node *link)
{
    // get the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // loop over the entries
    for (; entry != NULL; entry = SYMBOLTABLEENTRY_NEXT ( entry))
    {
        node *n = SYMBOLTABLEENTRY_LINK ( entry);

        // if (link == n) return entry;

        // printf("%ld = %ld\n", NODE_TYPE (link), NODE_TYPE (n));

        if (NODE_TYPE (link) != NODE_TYPE (n)) continue;

        if ( NODE_TYPE (n) == N_globdef) printf("%s, %s\n", GLOBDEF_NAME (n), GLOBDEF_NAME (link));
        if ( NODE_TYPE (n) == N_fundef) printf("%s, %s\n", FUNDEF_NAME (n), FUNDEF_NAME (link));
        if ( NODE_TYPE (n) == N_vardecl) printf("%s, %s\n", VARDECL_NAME (n), VARDECL_NAME (link));
        if ( NODE_TYPE (n) == N_param) printf("%s, %s\n", PARAM_NAME (n), PARAM_NAME (link));

        if ( NODE_TYPE (n) == N_globdef && STReq(GLOBDEF_NAME (n), GLOBDEF_NAME (link))) return entry;
        if ( NODE_TYPE (n) == N_fundef && STReq(FUNDEF_NAME (n), FUNDEF_NAME (link))) return entry;
        if ( NODE_TYPE (n) == N_vardecl && STReq(VARDECL_NAME (n), VARDECL_NAME (link))) return entry;
        if ( NODE_TYPE (n) == N_param && STReq(PARAM_NAME (n), PARAM_NAME (link))) return entry;

    }

    if (SYMBOLTABLE_PARENT ( table) == NULL) return NULL;

    return STdeepSearchByNode ( SYMBOLTABLE_PARENT ( table), link);
}

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearchFundef(node *table, const char *name)
{
    // the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // return the result
    return STsearchFundefEntry( entry, name);
}

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  list        the symbol table entry node
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearchFundefEntry(node *list, const char *name)
{
    // do we have a valid entry
    if ( list == NULL) return NULL;

    // skip fundefs
    if ( SYMBOLTABLEENTRY_TABLE (list) == NULL) return STsearchFundefEntry(SYMBOLTABLEENTRY_NEXT (list), name);

    // check if the name is the same
    if ( strcmp(SYMBOLTABLEENTRY_NAME (list), name) != 0) return STsearchFundefEntry(SYMBOLTABLEENTRY_NEXT (list), name);

    // return the result
    return list;
}

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @return node|NULL
 */
node *STdeepSearchFundef(node *table, const char *name)
{
    // search for the node in the current scope
    node *found = STsearchFundef( table, name);

    // do we have a node?
    if (found != NULL) return found;

    // get the parent table
    node *parent = SYMBOLTABLE_PARENT ( table);

    // do we have a parent table?
    if (parent == NULL) return NULL;

    // search for the node in the parent table
    return STdeepSearchFundef(parent, name);
}

/**
 *  Find the last entry in the list
 *  @param  table       the symbol table
 *  @return node|NULL
 */
node *STend(node *table)
{
    // call the method with the next entry
    return STlastEntry(SYMBOLTABLE_ENTRY (table));
}

/**
 *  Find the last entry in the list
 *  @param  list        the linked list
 *  @return node|NULL
 */
node *STlastEntry(node *list)
{
    // is the front set?
    if (list == NULL) return list;

    // is there a next entry?
    if (SYMBOLTABLEENTRY_NEXT (list) == NULL) return list;

    // call the method with the next entry
    return STlastEntry(SYMBOLTABLEENTRY_NEXT (list));
}


/**
 *  Number of external globdecls entries in the table
 *  @param  table   the symbol table
 *  @return size_t
 */
size_t STcountGlobdecls(node *table)
{
    // count
    size_t count = 0;

    // get the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // loop over the entries
    for (; entry != NULL; entry = SYMBOLTABLEENTRY_NEXT ( entry))
    {
        // get the node
        node *link = SYMBOLTABLEENTRY_LINK ( entry);

        // is it a globef node
        if ( NODE_TYPE (link) != N_globdef) continue;

        // is at en decl
        if ( !GLOBDEF_ISEXTERN (link)) continue;

        count++;
    }

    // return result
    return count;
}


/**
 *  Number of external fundecls entries in the table
 *  @param  table   the symbol table
 *  @return size_t
 */
size_t STcountFunDecls(node *table)
{
    // count
    size_t count = 0;

    // get the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // loop over the entries
    for (; entry != NULL; entry = SYMBOLTABLEENTRY_NEXT ( entry))
    {
        // get the node
        node *link = SYMBOLTABLEENTRY_LINK ( entry);

        // is it a fundef node
        if ( NODE_TYPE (link) != N_fundef) continue;

        // is at en decl
        if ( !FUNDEF_ISEXTERN (link)) continue;

        // increment the counter
        count++;
    }

    // return result
    return count;
}

/**
 *  Number of entries in the table
 *  @param  table   the symbol table
 *  @return size_t
 */
size_t STcount(node *table)
{
    // count
    size_t count = 0;

    // get the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // loop over the entries
    for (; entry != NULL; entry = SYMBOLTABLEENTRY_NEXT ( entry))
    {
        // get the node
        node *link = SYMBOLTABLEENTRY_LINK ( entry);

        // is it a globef node
        if ( NODE_TYPE (link) == N_fundef && FUNDEF_ISEXTERN (link)) continue;

        // is it a globef node
        if ( NODE_TYPE (link) == N_globdef && GLOBDEF_ISEXTERN (link)) continue;

        // increment the counter
        count++;
    }

    // return result
    return count;
}

/**
 *  The number of params
 *  @param  table   the symbol table
 *  @return size_t
 */
size_t STparams(node *table)
{
    // count
    size_t count = 0;

    // get the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // loop over the entries
    for (; entry != NULL; entry = SYMBOLTABLEENTRY_NEXT ( entry))
    {
        // do we have a param entry
        if (!SYMBOLTABLEENTRY_PARAM ( entry)) continue;

        // increment the count
        count++;
    }

    // return result
    return count;
}

/**
 *  The number of var decls
 *  @param  table   the symbol table
 *  @return size_t
 */
size_t STVardecls(node *table)
{
    // count
    size_t count = 0;

    // get the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // loop over the entries
    for (; entry != NULL; entry = SYMBOLTABLEENTRY_NEXT ( entry))
    {
        // do we have a param entry
        if (SYMBOLTABLEENTRY_PARAM ( entry)) continue;

        // increment the count
        count++;
    }

    // return result
    return count;
}

/**
 *  Check if the table is empty
 *  @param  table   the symbol table
 *  @return bool
 */
bool STempty(node *table)
{
    return SYMBOLTABLE_ENTRY (table) == NULL;
}

/**
 *  Add an entry to the list
 *  @param  table       the symbol table
 *  @param  type        new entry to add
 *  @return node|NULL
 */
node *STadd(node *table, node *entry)
{
    // check if we already have a similar node
    if (STsearchVariableByName( table, SYMBOLTABLEENTRY_NAME( entry)) != NULL) return NULL;

    // 
    if (STsearchFundef( table, SYMBOLTABLEENTRY_NAME( entry)) != NULL) return NULL;

    // get the node
    node *link = SYMBOLTABLEENTRY_LINK ( entry);

    // set the offset
    if ( NODE_TYPE ( link) == N_globdef && GLOBDEF_ISEXTERN ( link))SYMBOLTABLEENTRY_OFFSET( entry) = STcountGlobdecls ( table);
    else if ( NODE_TYPE (link) == N_fundef && FUNDEF_ISEXTERN ( link)) SYMBOLTABLEENTRY_OFFSET( entry) = STcountFunDecls ( table);
    else SYMBOLTABLEENTRY_OFFSET( entry) = STcount ( table);

    // find the last entry
    node *last = STend(table);

    // is the entry set at all?
    if (last == NULL) return SYMBOLTABLE_ENTRY ( table) = entry;

    // add the entry to the list
    return SYMBOLTABLEENTRY_NEXT (last) = entry;
}

/**
 *  Traverse over the entire symbol table
 *  @param  table       the table
 *  @param  callback    the callback
 */
void STtraverse(node *table, void callback( void *))
{
    STtraverseEntries(SYMBOLTABLE_ENTRY (table), callback);
}

/**
 *  Traverse over the entries symbol table
 *  @param  table       the table
 *  @param  callback    the callback
 */
void STtraverseEntries(node *list, void callback( void *))
{
    // do we have a valid entry
    if ( list == NULL) return;

    // call the call back
    callback( list);

    // get the table
    node *table = SYMBOLTABLEENTRY_TABLE( list);

    // doe we have a nested table?
    if (table != NULL) STtraverse(table, callback);

    // check if the name is the same
    STtraverseEntries(SYMBOLTABLEENTRY_NEXT (list), callback);
}

/**
 *  Display the symbol table
 *  @param  table   the symbol table
 *  @param  tabs    numbers of tabs to show
 */
void STdisplay(node *table, size_t tabs)
{
    STdisplayEntry ( SYMBOLTABLE_ENTRY ( table), tabs);
}

/**
 *  Display the symbol table
 *  @param  table   the symbol table
 *  @param  tabs    numbers of tabs to show
 */
void STdisplayEntry(node *list, size_t tabs)
{
    // do we have a node?
    if (list == NULL) return;

    // print the tabs
    for (size_t i=0; i < tabs; i++) printf("\t");

    // print the type
    printf("Type: ");
    switch (SYMBOLTABLEENTRY_TYPE ( list)) {
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

    // print the name
    printf(", Name: %s\n", SYMBOLTABLEENTRY_NAME ( list));

    // doe we have a nested table
    if (SYMBOLTABLEENTRY_TABLE ( list) != NULL) STdisplay(SYMBOLTABLEENTRY_TABLE ( list), tabs + 1);

    // display the next node
    STdisplayEntry (SYMBOLTABLEENTRY_NEXT ( list), tabs);
}
