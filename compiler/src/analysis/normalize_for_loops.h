#ifndef _NORMALIZE_FOR_LOOPS_H_
#define _NORMALIZE_FOR_LOOPS_H_

#include "types.h"

extern node *NFLdoNormalizeForLoops(node *syntaxtree);

extern node *NFLfor (node * arg_node, info * arg_info);
extern node *NFLfunbody (node * arg_node, info * arg_info);
// extern node *NFLfundef (node * arg_node, info * arg_info);
// extern node *NFLfundefs (node * arg_node, info * arg_info);

node *NFLvar(node * arg_node, info * arg_info);
node *NFLvarlet(node * arg_node, info * arg_info);
node *NFLvar(node * arg_node, info * arg_info);

#endif