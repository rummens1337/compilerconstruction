#ifndef _TRANSFORMATION_OF_CAST_EXPRESSIONS_H_
#define _TRANSFORMATION_OF_CAST_EXPRESSIONS_H_

#include "types.h"

extern node *TOCEdoTransformationOfCastExpressions(node *syntaxtree);

extern node *TOCEcast (node * arg_node, info * arg_info);

#endif