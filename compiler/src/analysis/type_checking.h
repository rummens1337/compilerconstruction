#ifndef _TYPE_CHECKING_H_
#define _TYPE_CHECKING_H_

#include "types.h"

extern node *TCdoTypeChecking(node *syntaxtree);

extern node *TCprogram (node * arg_node, info * arg_info);
extern node *TCfuncall (node * arg_node, info * arg_info);
extern node *TCfundef (node * arg_node, info * arg_info);
extern node *TCassign (node * arg_node, info * arg_info);
extern node *TCreturn (node * arg_node, info * arg_info);
extern node *TCexprstmt (node * arg_node, info * arg_info);
extern node *TCexprs (node * arg_node, info * arg_info);
extern node *TCvar (node * arg_node, info * arg_info);
extern node *TCbinop (node * arg_node, info * arg_info);
extern node *TCnum (node * arg_node, info * arg_info);
extern node *TCfloat (node * arg_node, info * arg_info);
extern node *TCbool (node * arg_node, info * arg_info);
extern node *TCcast (node * arg_node, info * arg_info);


#endif