#ifndef _COUNT_IO_H_
#define _COUNT_IO_H_

#include "types.h"

extern node *CIOvar (node *arg_node, info *arg_info);
extern node *CIOvarlet (node *arg_node, info *arg_info);
extern node *CIOmodule (node *arg_node, info *arg_info);
extern node *CIOdoCountAo( node *syntaxtree);

#endif
