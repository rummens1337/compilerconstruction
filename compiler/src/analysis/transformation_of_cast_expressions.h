#ifndef _TRANSFORMATION_OF_CAST_EXPRESSIONS_H_
#define _TRANSFORMATION_OF_CAST_EXPRESSIONS_H_

#include "types.h"

extern node *TOCEdoTransformationOfCastExpressions(node *syntaxtree);

extern node *TOCEprogram (node *arg_node, info *arg_info);
extern node *TOCEvar (node *arg_node, info *arg_info);
extern node *TOCEnum (node *arg_node, info *arg_info);
extern node *TOCEfloat (node *arg_node, info *arg_info);
extern node *TOCEbool (node *arg_node, info *arg_info);
extern node *TOCEcast (node * arg_node, info * arg_info);
extern node *TOCEfundef(node * arg_node, info * arg_info);
extern node *TOCEbinop (node * arg_node, info * arg_info);
#endif