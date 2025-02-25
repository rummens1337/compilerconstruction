
/**
 * @file eval.h
 *
 * Functions needed for compilation
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: free_node.h.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_EVAL_H_
#define _SAC_EVAL_H_

#include "types.h"

extern node *EVALassign (node * arg_node, info * arg_info);
extern node *EVALbinop (node * arg_node, info * arg_info);
extern node *EVALblock (node * arg_node, info * arg_info);
extern node *EVALbool (node * arg_node, info * arg_info);
extern node *EVALdeclarations (node * arg_node, info * arg_info);
extern node *EVALerror (node * arg_node, info * arg_info);
extern node *EVALexprs (node * arg_node, info * arg_info);
extern node *EVALfloat (node * arg_node, info * arg_info);
extern node *EVALfor (node * arg_node, info * arg_info);
extern node *EVALfunbody (node * arg_node, info * arg_info);
extern node *EVALfundec (node * arg_node, info * arg_info);
extern node *EVALfundef (node * arg_node, info * arg_info);
extern node *EVALfunheader (node * arg_node, info * arg_info);
extern node *EVALglobaldec (node * arg_node, info * arg_info);
extern node *EVALglobaldef (node * arg_node, info * arg_info);
extern node *EVALif (node * arg_node, info * arg_info);
extern node *EVALmodule (node * arg_node, info * arg_info);
extern node *EVALmonop (node * arg_node, info * arg_info);
extern node *EVALnum (node * arg_node, info * arg_info);
extern node *EVALparam (node * arg_node, info * arg_info);
extern node *EVALparams (node * arg_node, info * arg_info);
extern node *EVALprocall (node * arg_node, info * arg_info);
extern node *EVALreturn (node * arg_node, info * arg_info);
extern node *EVALstmts (node * arg_node, info * arg_info);
extern node *EVALsymboltableentry (node * arg_node, info * arg_info);
extern node *EVALvar (node * arg_node, info * arg_info);
extern node *EVALvardec (node * arg_node, info * arg_info);
extern node *EVALvardecs (node * arg_node, info * arg_info);
extern node *EVALvarlet (node * arg_node, info * arg_info);
extern node *EVALwhile (node * arg_node, info * arg_info);

#endif /* _SAC_EVAL_H_ */
