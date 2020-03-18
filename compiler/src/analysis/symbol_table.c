
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
static node *STsearchEntry(node *list, const char *name, type type);
static node *STsearchEntryByName(node *list, const char *name);
static node *STlastEntry(node *list);
static void STtraverseEntries(node *list, void callback( void *));

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearch(node *table, const char *name, type type)
{
    // the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // return the result
    return STsearchEntry( entry, name, type);
}

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  list        the symbol table entry node
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearchEntry(node *list, const char *name, type type)
{
    // do we have a valid entry
    if ( list == NULL) return NULL;

    // check if the name is the same
    if ( strcmp(SYMBOLTABLEENTRY_NAME (list), name) != 0) return STsearchEntry(SYMBOLTABLEENTRY_NEXT (list), name, type);

    // check if the type is the same
    if ( SYMBOLTABLEENTRY_TYPE (list) != type) return STsearchEntry(SYMBOLTABLEENTRY_NEXT (list), name, type);

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
node *STsearchByName(node *table, const char *name)
{
    // the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // return the result
    return STsearchEntryByName( entry, name);
}

/**
 *  Find an entry by its name in a linked list of Symbol Table Entry nodes
 *  @param  list        the symbol table entry node
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
node *STsearchEntryByName(node *list, const char *name)
{
    // do we have a valid entry
    if ( list == NULL) return NULL;

    // check if the name is the same
    if ( strcmp(SYMBOLTABLEENTRY_NAME (list), name) != 0) return STsearchEntryByName(SYMBOLTABLEENTRY_NEXT (list), name);

    // return the result
    return list;
}

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @return node|NULL
 */
node *STdeepSearchByName(node *table, const char *name)
{
    // the entry
    node *entry = SYMBOLTABLE_ENTRY ( table);

    // search for the node in the current scope
    node *found = STsearchEntryByName( entry, name);

    // do we have a node?
    if (found != NULL) return found;

    // get the parent table
    node *parent = SYMBOLTABLE_PARENT ( table);

    // do we have a parent table?
    if (parent == NULL) return NULL;

    // search for the node in the parent table
    return STdeepSearchByName(parent, name);
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
 *  Add an entry to the list
 *  @param  table       the symbol table
 *  @param  type        new entry to add
 *  @return node|NULL
 */
node *STadd(node *table, node *entry)
{
    // check if we already have a similar node
    if (STsearchByName( table, SYMBOLTABLEENTRY_NAME( entry)) != NULL) return NULL;

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