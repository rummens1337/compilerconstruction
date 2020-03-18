#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_

/**
 *  Dependencies
 */
#include "types.h"

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  list        the symbol table entry node
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
extern node *STsearch(node *list, const char *name, type type);

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
extern node *STsearchByName(node *table, const char *name);

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @return node|NULL
 */
node *STdeepSearchByName(node *table, const char *name);

/**
 *  Find the last entry in the list
 *  @param  list        the linked list
 *  @return node|NULL
 */
extern node *STlast(node *list);

/**
 *  Add an entry to the list
 *  @param  list        the linked list
 *  @param  type        new entry to add
 *  @return node|NULL
 */
extern node *STadd(node *list, node *entry);

/**
 *  Traverse over the entire symbol table
 *  @param  table       the table
 *  @param  callback    the callback
 */
extern void STtraverse(node *table, void callback( void *));


#endif