#ifndef _PROCESS_SYMBOL_TABLE_H_
#define _PROCESS_SYMBOL_TABLE_H_

#include "types.h"

extern node *PSTdoProcessSymbolTable(node *syntaxtree);

extern node *PSTprogram (node * arg_node, info * arg_info);
extern node *PSTfundef (node * arg_node, info * arg_info);
extern node *PSTglobdef (node * arg_node, info * arg_info);
extern node *PSTparam (node * arg_node, info * arg_info);
extern node *PSTfuncall (node * arg_node, info * arg_info);
extern node *PSTvardecl (node * arg_node, info * arg_info);
extern node *PSTvarlet (node * arg_node, info * arg_info);
extern node *PSTvar (node * arg_node, info * arg_info);

#endif