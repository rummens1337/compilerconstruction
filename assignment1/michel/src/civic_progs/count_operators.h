#ifndef _COUNT_OPERATORS_H_
#define _COUNT_OPERATORS_H_
#include "types.h"

extern node *CObinop (node *arg_node, info *arg_info);
extern node *COmodule (node *arg_node, info *arg_info);
extern node *COdoCountOperators(node *syntaxtree);

#endif