#ifndef _CHANGE_VARIABLE_INITIALISATIONS_H_
#define _CHANGE_VARIABLE_INITIALISATIONS_H_

#include "types.h"

extern node *CVIfunbody (node * arg_node, info * arg_info);
extern node *CVIglobdef (node * arg_node, info * arg_info);
extern node *CVIvardecl (node * arg_node, info * arg_info);
extern node *CVIdecls (node * arg_node, info * arg_info);



extern node *CVIdoChangeVariableInitialisations(node *syntaxtree);

#endif