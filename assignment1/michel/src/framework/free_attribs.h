
/**
 * @file free_attribs.h
 *
 * Functions to free the attributes of node structures
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: free_attribs.h.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_FREE_ATTRIBS_H_
#define _SAC_FREE_ATTRIBS_H_

#include "types.h"

extern char *FREEattribString (char *attr, node * parent);
extern node *FREEattribLink (node * attr, node * parent);

#endif /* _SAC_FREE_ATTRIBS_H_ */
