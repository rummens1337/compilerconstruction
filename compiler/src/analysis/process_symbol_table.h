#ifndef _PROCESS_SYMBOL_TABLE_H_
#define _PROCESS_SYMBOL_TABLE_H_

#include "types.h"

extern node *PSTdoProcessSymbolTable(node *syntaxtree);


extern node *PSTarrexpr (node * arg_node, info * arg_info);
extern node *PSTassign (node * arg_node, info * arg_info);
extern node *PSTbinop (node * arg_node, info * arg_info);
extern node *PSTbool (node * arg_node, info * arg_info);
extern node *PSTcast (node * arg_node, info * arg_info);
extern node *PSTdecls (node * arg_node, info * arg_info);
extern node *PSTdowhile (node * arg_node, info * arg_info);
extern node *PSTerror (node * arg_node, info * arg_info);
extern node *PSTexprstmt (node * arg_node, info * arg_info);
extern node *PSTexprs (node * arg_node, info * arg_info);
extern node *PSTfloat (node * arg_node, info * arg_info);
extern node *PSTfor (node * arg_node, info * arg_info);
extern node *PSTfunbody (node * arg_node, info * arg_info);
extern node *PSTfuncall (node * arg_node, info * arg_info);
extern node *PSTfundef (node * arg_node, info * arg_info);
extern node *PSTfundefs (node * arg_node, info * arg_info);
extern node *PSTglobdecl (node * arg_node, info * arg_info);
extern node *PSTglobdef (node * arg_node, info * arg_info);
extern node *PSTids (node * arg_node, info * arg_info);
extern node *PSTifelse (node * arg_node, info * arg_info);
extern node *PSTmonop (node * arg_node, info * arg_info);
extern node *PSTnum (node * arg_node, info * arg_info);
extern node *PSTparam (node * arg_node, info * arg_info);
extern node *PSTprogram (node * arg_node, info * arg_info);
extern node *PSTreturn (node * arg_node, info * arg_info);
extern node *PSTstmts (node * arg_node, info * arg_info);
extern node *PSTvar (node * arg_node, info * arg_info);
extern node *PSTvardecl (node * arg_node, info * arg_info);
extern node *PSTvarlet (node * arg_node, info * arg_info);
extern node *PSTwhile (node * arg_node, info * arg_info);
extern node *PSTsymboltable (node * arg_node, info * arg_info);
extern node *PSTsymboltableentry (node * arg_node, info * arg_info);

#endif