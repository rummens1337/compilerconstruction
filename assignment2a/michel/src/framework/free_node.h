
/**
 * @file free_node.h
 *
 * Functions to free node structures
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: free_node.h.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_FREE_NODE_H_
#define _SAC_FREE_NODE_H_

#include "types.h"

extern node *FREEassign (node * arg_node, info * arg_info);
extern node *FREEbinop (node * arg_node, info * arg_info);
extern node *FREEblock (node * arg_node, info * arg_info);
extern node *FREEbool (node * arg_node, info * arg_info);
extern node *FREEdeclarations (node * arg_node, info * arg_info);
extern node *FREEerror (node * arg_node, info * arg_info);
extern node *FREEexprs (node * arg_node, info * arg_info);
extern node *FREEfloat (node * arg_node, info * arg_info);
extern node *FREEfor (node * arg_node, info * arg_info);
extern node *FREEfunbody (node * arg_node, info * arg_info);
extern node *FREEfundec (node * arg_node, info * arg_info);
extern node *FREEfundef (node * arg_node, info * arg_info);
extern node *FREEfunheader (node * arg_node, info * arg_info);
extern node *FREEglobaldec (node * arg_node, info * arg_info);
extern node *FREEglobaldef (node * arg_node, info * arg_info);
extern node *FREEif (node * arg_node, info * arg_info);
extern node *FREEmodule (node * arg_node, info * arg_info);
extern node *FREEmonop (node * arg_node, info * arg_info);
extern node *FREEnum (node * arg_node, info * arg_info);
extern node *FREEparam (node * arg_node, info * arg_info);
extern node *FREEparams (node * arg_node, info * arg_info);
extern node *FREEprocall (node * arg_node, info * arg_info);
extern node *FREEreturn (node * arg_node, info * arg_info);
extern node *FREEstmts (node * arg_node, info * arg_info);
extern node *FREEsymboltableentry (node * arg_node, info * arg_info);
extern node *FREEvar (node * arg_node, info * arg_info);
extern node *FREEvardec (node * arg_node, info * arg_info);
extern node *FREEvardecs (node * arg_node, info * arg_info);
extern node *FREEvarlet (node * arg_node, info * arg_info);
extern node *FREEwhile (node * arg_node, info * arg_info);

#endif /* _SAC_FREE_NODE_H_ */
