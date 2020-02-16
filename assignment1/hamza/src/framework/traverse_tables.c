
/**
 * @file traverse_tables.c
 *
 * This file defines the function tables for traversal.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: traverse_tables.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#include "traverse_tables.h"
#include "traverse_helper.h"
#include "print.h"
#include "copy_node.h"
#include "free_node.h"
#include "check.h"
#include "rename_identifiers.h"
#include "sum_ints.h"
#include "opt_sub.h"
#include "count_ao.h"
#include "count_io.h"


travtables_t travtables = {
  /* TR_undefined */
  {&TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror}

  /* TR_prt */
  , {&TRAVerror, &PRTmodule, &PRTstmts, &PRTassign, &PRTbinop, &PRTvarlet,
     &PRTvar, &PRTnum, &PRTfloat, &PRTbool, &PRTsymboltableentry, &PRTerror}

  /* TR_copy */
  , {&TRAVerror, &COPYmodule, &COPYstmts, &COPYassign, &COPYbinop,
     &COPYvarlet, &COPYvar, &COPYnum, &COPYfloat, &COPYbool,
     &COPYsymboltableentry, &COPYerror}

  /* TR_free */
  , {&TRAVerror, &FREEmodule, &FREEstmts, &FREEassign, &FREEbinop,
     &FREEvarlet, &FREEvar, &FREEnum, &FREEfloat, &FREEbool,
     &FREEsymboltableentry, &FREEerror}

  /* TR_chk */
  , {&TRAVerror, &CHKmodule, &CHKstmts, &CHKassign, &CHKbinop, &CHKvarlet,
     &CHKvar, &CHKnum, &CHKfloat, &CHKbool, &CHKsymboltableentry, &CHKerror}

  /* TR_ri */
  , {&TRAVerror, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &RIvarlet,
     &RIvar, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_si */
  , {&TRAVerror, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &SInum, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_os */
  , {&TRAVerror, &TRAVsons, &TRAVsons, &TRAVsons, &OSbinop, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_cao */
  , {&TRAVerror, &CAOmodule, &CAOstmts, &TRAVsons, &CAObinop, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_cio */
  , {&TRAVerror, &CIOmodule, &TRAVsons, &TRAVsons, &TRAVsons, &CIOvarlet,
     &CIOvar, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}
};

preposttable_t pretable = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

preposttable_t posttable = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

const char *travnames[10] = {
  "unknown", "prt", "copy", "free", "chk", "ri", "si", "os", "cao", "cio"
};
