#ifndef _CHECK_RANGE_H_
#define _CHECK_RANGE_H_

#include "types.h"

extern node *CRdoCheckRange(node *syntaxtree);

extern node *CRprogram (node *arg_node, info *arg_info);
extern node *CRvar (node *arg_node, info *arg_info);
extern node *CRnum (node *arg_node, info *arg_info);
extern node *CRfloat (node *arg_node, info *arg_info);
extern node *CRbool (node *arg_node, info *arg_info);
extern node *CRcast (node * arg_node, info * arg_info);
extern node *CRbinop (node * arg_node, info * arg_info);
#endif