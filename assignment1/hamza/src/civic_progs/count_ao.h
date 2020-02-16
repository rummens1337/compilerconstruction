#ifndef _COUNT_AO_H_
#define _COUNT_AO_H_

#include "types.h"

extern node *CAObinop (node *arg_node, info *arg_info);
extern node *CAOstmts (node *arg_node, info *arg_info);
extern node *CAOmodule (node *arg_node, info *arg_info);
extern node *CAOdoCountAo( node *syntaxtree);

#endif
