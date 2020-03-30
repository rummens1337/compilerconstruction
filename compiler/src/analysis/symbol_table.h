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
extern node *STsearchVariable(node *list, const char *name, type type);
extern node *STsearchVariableByName(node *table, const char *name);

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @param  type        the type of node
 *  @return node|NULL
 */
extern node *STsearchFundef(node *table, const char *name);

/**
 *  Find an entry  by its name in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  name        the name of the node
 *  @return node|NULL
 */
extern node *STdeepSearchVariableByName(node *table, const char *name);
extern node *STdeepSearchFundef(node *table, const char *name);


/**
 *  Find the last entry in the list
 *  @param  list        the linked list
 *  @return node|NULL
 */
extern node *STend(node *list);

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

/**
 *  The current offset of a table
 *  @param  table   the symbol table
 *  @return int
 */
extern int SToffset(node *table);

/**
 *  Find an entry in a linked list of Symbol Table Entry nodes
 *  @param  table       the symbol table
 *  @param  offset      the offset
 *  @return node|NULL
 */
extern node *STget(node *table, int offset);

/**
 *  Display the symbol table
 *  @param  table   the symbol table
 *  @param  tabs    numbers of tabs to show
 */
extern void STdisplay(node *table, size_t tabs);

/**
 *  Number of entries in the table
 *  @param  table   the symbol table
 *  @return size_t
 */
size_t STcount(node *table);

/**
 *  The number of params
 *  @param  table   the symbol table
 */
extern size_t STparams();

#endif