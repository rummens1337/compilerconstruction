#ifndef _TYPE_CHECKER_H_
#define _TYPE_CHECKER_H_
#include "types.h"

extern node *TCassign(node *arg_node, info *arg_info);
extern node *TCifelse(node *arg_node, info *arg_info);
extern node *TCwhile(node *arg_node, info *arg_info);
extern node *TCdowhile(node *arg_node, info *arg_info);
extern node *TCfor(node *arg_node, info *arg_info);

extern node *TCdoTypeChecker(node *syntaxtree);

#endif