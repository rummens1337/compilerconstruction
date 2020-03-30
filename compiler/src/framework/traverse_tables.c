
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
#include "process_symbol_table.h"
#include "normalize_for_loops.h"
#include "change_variable_initialisations.h"
#include "compiling_boolean_disjunction_and_conjunction.h"
#include "transformation_of_cast_expressions.h"
#include "type_checking.h"


travtables_t travtables = {
  /* TR_undefined */
  {&TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror,
   &TRAVerror, &TRAVerror, &TRAVerror, &TRAVerror}

  /* TR_prt */
  , {&TRAVerror, &PRTprogram, &PRTsymboltable, &PRTsymboltableentry,
     &PRTdecls, &PRTexprs, &PRTarrexpr, &PRTids, &PRTexprstmt, &PRTreturn,
     &PRTfuncall, &PRTcast, &PRTfundefs, &PRTfundef, &PRTfunbody, &PRTifelse,
     &PRTternary, &PRTwhile, &PRTdowhile, &PRTfor, &PRTglobdecl, &PRTglobdef,
     &PRTparam, &PRTvardecl, &PRTstmts, &PRTassign, &PRTbinop, &PRTmonop,
     &PRTvarlet, &PRTvar, &PRTnum, &PRTfloat, &PRTbool, &PRTerror}

  /* TR_copy */
  , {&TRAVerror, &COPYprogram, &COPYsymboltable, &COPYsymboltableentry,
     &COPYdecls, &COPYexprs, &COPYarrexpr, &COPYids, &COPYexprstmt,
     &COPYreturn, &COPYfuncall, &COPYcast, &COPYfundefs, &COPYfundef,
     &COPYfunbody, &COPYifelse, &COPYternary, &COPYwhile, &COPYdowhile,
     &COPYfor, &COPYglobdecl, &COPYglobdef, &COPYparam, &COPYvardecl,
     &COPYstmts, &COPYassign, &COPYbinop, &COPYmonop, &COPYvarlet, &COPYvar,
     &COPYnum, &COPYfloat, &COPYbool, &COPYerror}

  /* TR_free */
  , {&TRAVerror, &FREEprogram, &FREEsymboltable, &FREEsymboltableentry,
     &FREEdecls, &FREEexprs, &FREEarrexpr, &FREEids, &FREEexprstmt,
     &FREEreturn, &FREEfuncall, &FREEcast, &FREEfundefs, &FREEfundef,
     &FREEfunbody, &FREEifelse, &FREEternary, &FREEwhile, &FREEdowhile,
     &FREEfor, &FREEglobdecl, &FREEglobdef, &FREEparam, &FREEvardecl,
     &FREEstmts, &FREEassign, &FREEbinop, &FREEmonop, &FREEvarlet, &FREEvar,
     &FREEnum, &FREEfloat, &FREEbool, &FREEerror}

  /* TR_chk */
  , {&TRAVerror, &CHKprogram, &CHKsymboltable, &CHKsymboltableentry,
     &CHKdecls, &CHKexprs, &CHKarrexpr, &CHKids, &CHKexprstmt, &CHKreturn,
     &CHKfuncall, &CHKcast, &CHKfundefs, &CHKfundef, &CHKfunbody, &CHKifelse,
     &CHKternary, &CHKwhile, &CHKdowhile, &CHKfor, &CHKglobdecl, &CHKglobdef,
     &CHKparam, &CHKvardecl, &CHKstmts, &CHKassign, &CHKbinop, &CHKmonop,
     &CHKvarlet, &CHKvar, &CHKnum, &CHKfloat, &CHKbool, &CHKerror}

  /* TR_pst */
  , {&TRAVerror, &PSTprogram, &TRAVsons, &TRAVsons, &TRAVsons, &PSTexprs,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &PSTfuncall, &TRAVsons,
     &TRAVsons, &PSTfundef, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &PSTglobdef, &PSTparam, &PSTvardecl,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &PSTvarlet, &PSTvar,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_nfl */
  , {&TRAVerror, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &NFLfunbody, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &NFLfor, &TRAVsons, &TRAVsons, &TRAVsons, &NFLvardecl,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &NFLvarlet, &NFLvar,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_cvi */
  , {&TRAVerror, &CVIprogram, &TRAVsons, &TRAVsons, &CVIdecls, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &CVIfunbody, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &CVIglobdef, &TRAVsons, &CVIvardecl,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_cbdc */
  , {&TRAVerror, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &CBDCbinop, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_toce */
  , {&TRAVerror, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TOCEcast,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons}

  /* TR_tc */
  , {&TRAVerror, &TCprogram, &TRAVsons, &TRAVsons, &TRAVsons, &TCexprs,
     &TRAVsons, &TRAVsons, &TCexprstmt, &TCreturn, &TCfuncall, &TCcast,
     &TRAVsons, &TCfundef, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons, &TRAVsons,
     &TRAVsons, &TCassign, &TCbinop, &TRAVsons, &TRAVsons, &TCvar, &TCnum,
     &TCfloat, &TCbool, &TRAVsons}
};

preposttable_t pretable = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

preposttable_t posttable = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

const char *travnames[11] = {
  "unknown", "prt", "copy", "free", "chk", "pst", "nfl", "cvi", "cbdc",
    "toce", "tc"
};
