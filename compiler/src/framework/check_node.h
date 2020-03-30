
/**
 * @file check_node.h
 *
 * Functions to free node structures
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: check_node.h.xsl 15657 2007-11-13 13:57:30Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_CHECK_NODE_H_
#define _SAC_CHECK_NODE_H_

#include "types.h"

extern node *CHKMpostfun (node * arg_node, info * arg_info);

extern node *CHKMarrexpr (node * arg_node, info * arg_info);
extern node *CHKMassign (node * arg_node, info * arg_info);
extern node *CHKMbinop (node * arg_node, info * arg_info);
extern node *CHKMbool (node * arg_node, info * arg_info);
extern node *CHKMcast (node * arg_node, info * arg_info);
extern node *CHKMdecls (node * arg_node, info * arg_info);
extern node *CHKMdowhile (node * arg_node, info * arg_info);
extern node *CHKMerror (node * arg_node, info * arg_info);
extern node *CHKMexprstmt (node * arg_node, info * arg_info);
extern node *CHKMexprs (node * arg_node, info * arg_info);
extern node *CHKMfloat (node * arg_node, info * arg_info);
extern node *CHKMfor (node * arg_node, info * arg_info);
extern node *CHKMfunbody (node * arg_node, info * arg_info);
extern node *CHKMfuncall (node * arg_node, info * arg_info);
extern node *CHKMfundef (node * arg_node, info * arg_info);
extern node *CHKMfundefs (node * arg_node, info * arg_info);
extern node *CHKMglobdecl (node * arg_node, info * arg_info);
extern node *CHKMglobdef (node * arg_node, info * arg_info);
extern node *CHKMids (node * arg_node, info * arg_info);
extern node *CHKMifelse (node * arg_node, info * arg_info);
extern node *CHKMmonop (node * arg_node, info * arg_info);
extern node *CHKMnum (node * arg_node, info * arg_info);
extern node *CHKMparam (node * arg_node, info * arg_info);
extern node *CHKMprogram (node * arg_node, info * arg_info);
extern node *CHKMreturn (node * arg_node, info * arg_info);
extern node *CHKMstmts (node * arg_node, info * arg_info);
extern node *CHKMsymboltable (node * arg_node, info * arg_info);
extern node *CHKMsymboltableentry (node * arg_node, info * arg_info);
extern node *CHKMternary (node * arg_node, info * arg_info);
extern node *CHKMvar (node * arg_node, info * arg_info);
extern node *CHKMvardecl (node * arg_node, info * arg_info);
extern node *CHKMvarlet (node * arg_node, info * arg_info);
extern node *CHKMwhile (node * arg_node, info * arg_info);

#endif /* _SAC_CHECK_NODE_H_ */
