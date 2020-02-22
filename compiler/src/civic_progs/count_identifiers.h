#ifndef _COUNT_IDENTIFIERS_H_
#define _COUNT_IDENTIFIERS_H_

#include "types.h"

extern node *CIvar(node *arg_node, info *arg_info);

extern node *CIvarlet(node *arg_node, info *arg_info);

extern node *CIdoCountIdentifiers(node *syntaxtree);

#endif