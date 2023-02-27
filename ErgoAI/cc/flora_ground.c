/* File:      flora_ground.c
** Author(s): kifer
**
** Contact:   see  ../CONTACTS.txt
**
** Copyright (C) by
**      The Research Foundation of the State University of New York, 2012-2018;
**      and Vulcan, Inc., 2012-2013.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
**
*/


#if 0
#define FG_DEBUG
#endif

#include "xsb_config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef WIN_NT
#define XSB_DLL
#endif

#include "builtin.h"
#include "auxlry.h"
#include "context.h"
#include "cell_xsb.h"
#include "error_xsb.h"
#include "cinterf.h"
#include "deref.h"
#include "memory_xsb.h"

#include "flora_cc_prefix.h"

#define FLORA_NAF_PREDICATE       "flora_naf"
#define FLORA_NAF_ARITY           4
#define FLORA_NAF_LEN             9

#define FL_TRUTHVALUE_TABLED_CALL "truthvalue_tabled_call"
#define FL_TABLED_NAF_CALL        "tabled_naf_call"
#define FL_UNDEFEATED             "undefeated"


inline static int is_flora_form(prolog_term term, Integer ignore_negative);
inline static int is_flora_tnot_predicate(prolog_term pterm);
static int is_ignorable_functor(prolog_term pterm);
inline static int local_ground(CPtr term);
inline static int local_ground_cyc(CTXTc Cell term, int cycle_action);
inline static prolog_term trim(CPtr pterm);
inline static prolog_term trim_compound(prolog_term pterm, int arity);
inline static prolog_term trim_list(prolog_term inList);
inline static void term_vars(CPtr pterm,
			     CPtr* pvars, CPtr* pvarstail,
			     Integer ignore_negative);
inline static void term_vars_split(CPtr pterm,
				   CPtr* pvars, CPtr* pvarstail,
				   CPtr* pattrvars, CPtr* pattrvarstail,
				   Integer ignore_negative);
//extern xsbBool is_cyclic(CTXTdeclc Cell);


#ifdef FG_DEBUG
static char *pterm2string(CTXTdeclc prolog_term term);
#endif

DllExport xsbBool call_conv flratom_char_code (CTXTdecl)
{
  char *inatom = ptoc_string(CTXTc 1);
  Integer pos = ptoc_int(CTXTc 2);
  Integer charcode = inatom[pos];
  prolog_term pcode = p2p_new();
  prolog_term out = extern_reg_term(3);
  
  c2p_int(CTXTc charcode,pcode);
  return extern_p2p_unify(pcode,out);
}

DllExport xsbBool call_conv flratom_begins_with (CTXTdecl)
{
  char *big = ptoc_string(CTXTc 1);
  char *small = ptoc_string(CTXTc 2);

  while(*big != '\0' && *small != '\0') {
    if (*big != *small) return FALSE;
    big++;
    small++;
  }

  if (*small == '\0') return TRUE;
  return FALSE;
}


DllExport xsbBool call_conv flratom_ends_with (CTXTdecl)
{
  char *big = ptoc_string(CTXTc 1);
  char *small = ptoc_string(CTXTc 2);
  int  difflen = (int) strlen(big) - (int) strlen(small);
  //size_t  difflen = strlen(big) - strlen(small);

  if (difflen < 0) return FALSE;

  big = big+difflen;
  while(*big != '\0' && *small != '\0') {
    if (*big != *small) return FALSE;
    big++;
    small++;
  }

  if (*small == '\0') return TRUE;
  return FALSE;
}

DllExport xsbBool call_conv flrground (CTXTdecl)
{
  prolog_term pterm = extern_reg_term(1);

  return local_ground((CPtr) pterm);
}

DllExport xsbBool call_conv flrnonground (CTXTdecl)
{
  prolog_term pterm = extern_reg_term(1);

  return !local_ground((CPtr) pterm);
}

DllExport xsbBool call_conv flrground_cyc(CTXTdecl)
{ 
  prolog_term pterm = extern_reg_term(1);

  return local_ground_cyc(CTXTc (Cell) pterm,CYCLIC_FAIL);
}

DllExport xsbBool call_conv flrnonground_cyc(CTXTdecl)
{ 
  prolog_term pterm = extern_reg_term(1);

  // we want to fail on cyclic terms always
  return !local_ground_cyc(CTXTc (Cell) pterm,CYCLIC_SUCCEED);
}



/* If Arg 3 is != 0, ignore vars in negative subgoals and in
   true{..}/false{..}, ...
*/
DllExport xsbBool call_conv flrterm_vars (CTXTdecl)
{
  prolog_term pterm = extern_reg_term(1);
  prolog_term outvars = extern_reg_term(2);
  Integer ignore_negative = extern_ptoc_int(3);
  prolog_term vars = extern_p2p_new();
  prolog_term tail = vars;

#ifdef FG_DEBUG
  fprintf(stderr,"term_vars: pterm=%s\n", pterm2string(CTXTc pterm));
#endif

  term_vars((CPtr) pterm, (CPtr *) &vars, (CPtr*) &tail, ignore_negative);
  
#ifdef FG_DEBUG
  fprintf(stderr,"term_vars2: vars=%s\n", pterm2string(CTXTc vars));
  fprintf(stderr,"term_vars2: tail=%s\n", pterm2string(CTXTc tail));
#endif

  extern_c2p_nil(tail);
  return extern_p2p_unify(vars,outvars);
}


/* If Arg 4 is != 0, ignore vars in negative subgoals and in
   true{..}/false{..}, ...
*/
DllExport xsbBool call_conv flrterm_vars_split (CTXTdecl)
{
  prolog_term pterm = extern_reg_term(1);
  prolog_term outvars = extern_reg_term(2);
  prolog_term outattrvars = extern_reg_term(3);
  Integer ignore_negative = extern_ptoc_int(4);
  prolog_term vars = extern_p2p_new();
  prolog_term tail = vars;
  prolog_term attrvars = extern_p2p_new();
  prolog_term attrtail = attrvars;

#ifdef FG_DEBUG
  fprintf(stderr,"term_vars_split: pterm=%s\n", pterm2string(CTXTc pterm));
#endif

  /* we should really check for acyclicity while traversing pterm in
     term_vars_split */
  //if (is_cyclic(CTXTc (Cell) pterm)) return FALSE;
  term_vars_split((CPtr)pterm,
		  (CPtr *)&vars, (CPtr*)&tail,
		  (CPtr *)&attrvars, (CPtr*)&attrtail,
		  ignore_negative);
  
#ifdef FG_DEBUG
  fprintf(stderr,"term_vars_split2: vars=%s\n", pterm2string(CTXTc vars));
  fprintf(stderr,"term_vars_split2: tail=%s\n", pterm2string(CTXTc tail));
#endif

  extern_c2p_nil(tail);
  extern_c2p_nil(attrtail);
  return (extern_p2p_unify(vars,outvars) && extern_p2p_unify(attrvars,outattrvars));
}


// for flora predicates, don't check the last argument for groundedness because
// it is _$ctxt(_,_,_)
int local_ground(CPtr pterm)
{
  int j, arity;

 groundBegin:
  XSB_CptrDeref(pterm);
  switch(cell_tag(pterm)) {
  case XSB_FREE:
  case XSB_REF1:
  case XSB_ATTV:
    return FALSE;

  case XSB_STRING:
  case XSB_INT:
  case XSB_FLOAT:
    return TRUE;

  case XSB_LIST:
    if (!local_ground(clref_val(pterm)))
      return FALSE;
    pterm = clref_val(pterm)+1;
    goto groundBegin;

  case XSB_STRUCT:
    arity = (int) get_arity(get_str_psc(pterm));
    if (arity == 0) return TRUE;
    for (j=1; j < arity ; j++)
      if (!local_ground(clref_val(pterm)+j))
	return FALSE;
    // if it is a flora predicate, ignore the last argument number=arity
    // and don't check it for groundedness
    if (is_flora_form((prolog_term)pterm,1)) // ignore negative
      return TRUE;

    pterm = clref_val(pterm)+arity;
    goto groundBegin;

  default:
    xsb_abort("[FLORA]: BUG in flrground/1: term with unknown tag (%d)",
	      (int)cell_tag(pterm));
    return FALSE;	/* so that g++ does not complain */
  }
}


DllExport xsbBool call_conv flrtrim_last(CTXTdecl)
{
  prolog_term pterm = extern_reg_term(1);
  prolog_term trimmedterm = extern_reg_term(2);

  return extern_p2p_unify(trimmedterm, trim((CPtr) pterm));
  return TRUE;
}

prolog_term trim(CPtr pterm)
{
  int arity;
  XSB_CptrDeref(pterm);
  switch(cell_tag(pterm)) {
  case XSB_FREE:
  case XSB_REF1:
  case XSB_ATTV:
  case XSB_STRING:
  case XSB_INT:
  case XSB_FLOAT:
    return (prolog_term)pterm;

  case XSB_LIST:
    return trim_list((prolog_term)pterm);

  case XSB_STRUCT:
    arity = (int) get_arity(get_str_psc(pterm));
    if (arity == 0) return (prolog_term)pterm;
    return trim_compound((prolog_term)pterm,arity);

  default:
    xsb_abort("[FLORA]: internal bug (flrtrim_last/2): term with unknown tag (%d)",
	      (int)cell_tag(pterm));
    return FALSE;
  }
}


void term_vars(CPtr pterm, CPtr* pvars, CPtr* pvarstail, Integer ignore_negative)
{
  int j, arity;

 groundBegin:
  XSB_CptrDeref(pterm);
  switch(cell_tag(pterm)) {
  case XSB_FREE:
  case XSB_REF1:
  case XSB_ATTV:
#ifdef FG_DEBUG
    fprintf(stderr,"v1: Arg1=%s\n",pterm2string(CTXTc (prolog_term)pterm));
    fprintf(stderr,"v1: Arg2=%s\n",pterm2string(CTXTc (prolog_term)*pvars));
    fprintf(stderr,"v1: Arg3=%s\n",pterm2string(CTXTc (prolog_term)*pvarstail));
#endif

    extern_c2p_list((prolog_term) *pvarstail);
    extern_p2p_unify((prolog_term) pterm,
		     extern_p2p_car((prolog_term) *pvarstail));
    pvars = (CPtr *) pvarstail;
    *pvarstail = (CPtr) extern_p2p_cdr((prolog_term) *pvarstail);
#ifdef FG_DEBUG
    fprintf(stderr,"v2: Arg1=%s\n",pterm2string(CTXTc (prolog_term)pterm));
    fprintf(stderr,"v2: Arg2=%s\n",pterm2string(CTXTc (prolog_term)*pvars));
    fprintf(stderr,"v2: Arg3=%s\n",pterm2string(CTXTc (prolog_term)*pvarstail));
#endif
    return;

  case XSB_STRING:
  case XSB_INT:
  case XSB_FLOAT:
    return;

  case XSB_LIST:
    term_vars(clref_val(pterm),pvars,pvarstail,ignore_negative);
    pterm = clref_val(pterm)+1;
    goto groundBegin;

  case XSB_STRUCT:
#ifdef FG_DEBUG
      fprintf(stderr,"pterm: %s\n",pterm2string(CTXTc (prolog_term)pterm));
#endif
    arity = (int) get_arity(get_str_psc(pterm));
    // if it is FLORA_NAF_PREDICATE(Call,FreeVars,File,Line), get vars from Call only
    if (is_flora_tnot_predicate((prolog_term) pterm)
	&& arity == FLORA_NAF_ARITY) {
      term_vars(clref_val(pterm)+1,pvars,pvarstail,ignore_negative);
#ifdef FG_DEBUG
      fprintf(stderr,"pvars: %s\n",
              pterm2string(CTXTc (prolog_term) *pvars));
      fprintf(stderr,"pvarstail: %s\n",
              pterm2string(CTXTc (prolog_term) *pvarstail));
#endif
      return;
    }
    if (is_ignorable_functor((prolog_term) pterm))
	return;
    if (arity == 0)
      return;
    for (j=1; j < arity; j++) {
#ifdef FG_DEBUG
      fprintf(stderr,"strct: Arg1=%s\n",
              pterm2string(CTXTc (prolog_term) *(clref_val(pterm)+j)));
      fprintf(stderr,"strct: Arg2=%s\n",
              pterm2string(CTXTc (prolog_term) *pvars));
      fprintf(stderr,"strct: Arg3=%s\n",
              pterm2string(CTXTc (prolog_term) *pvarstail));
#endif

      term_vars(clref_val(pterm)+j,pvars,pvarstail,ignore_negative);

#ifdef FG_DEBUG
      fprintf(stderr,"strct2: Arg1=%s\n",
              pterm2string(CTXTc (prolog_term) *(clref_val(pterm)+j)));
      fprintf(stderr,"strct2: Arg2=%s\n",
              pterm2string(CTXTc (prolog_term) *pvars));
      fprintf(stderr,"strct2: Arg3=%s\n",
              pterm2string(CTXTc (prolog_term) *pvarstail));
#endif
    }
    // if this is a flora formula, no need to check the last argument
    if (is_flora_form((prolog_term)pterm,ignore_negative))
      return;

    pterm = clref_val(pterm)+arity;
    goto groundBegin;

  default:
    xsb_abort("[FLORA]: BUG in flrterm_vars/2: term with unknown tag (%d)",
	      (int)cell_tag(pterm));
    return;
  }
}



// this one splits attributed from regular vars
void term_vars_split(CPtr pterm,
		     CPtr* pvars, CPtr* pvarstail,
		     CPtr* pattrvars, CPtr* pattrvarstail,
		     Integer ignore_negative)
{
  int j, arity;

 groundBegin:
  XSB_CptrDeref(pterm);
  switch(cell_tag(pterm)) {
  case XSB_FREE:
  case XSB_REF1:
    extern_c2p_list((prolog_term) *pvarstail);
    extern_p2p_unify((prolog_term) pterm,
		     extern_p2p_car((prolog_term) *pvarstail));
    pvars = (CPtr *) pvarstail;
    *pvarstail = (CPtr) extern_p2p_cdr((prolog_term) *pvarstail);
    return;

  case XSB_ATTV:
    extern_c2p_list((prolog_term) *pattrvarstail);
    extern_p2p_unify((prolog_term) pterm,
		     extern_p2p_car((prolog_term) *pattrvarstail));
    pattrvars = (CPtr *) pattrvarstail;
    *pattrvarstail = (CPtr) extern_p2p_cdr((prolog_term) *pattrvarstail);
    return;

  case XSB_STRING:
  case XSB_INT:
  case XSB_FLOAT:
    return;

  case XSB_LIST:
    term_vars_split(clref_val(pterm),pvars,pvarstail,pattrvars,pattrvarstail,ignore_negative);
    pterm = clref_val(pterm)+1;
    goto groundBegin;

  case XSB_STRUCT:
    arity = (int) get_arity(get_str_psc(pterm));
    // if it is FLORA_NAF_PREDICATE(Call,FreeVars,File,Line), get vars from Call only
    if (is_flora_tnot_predicate((prolog_term) pterm)
	&& arity == FLORA_NAF_ARITY) {
      term_vars_split(clref_val(pterm)+1,pvars,pvarstail,pattrvars,pattrvarstail,ignore_negative);
      return;
    }
    if (is_ignorable_functor((prolog_term) pterm))
	return;
    if (arity == 0)
      return;
    for (j=1; j < arity; j++) {
      term_vars_split(clref_val(pterm)+j,pvars,pvarstail,pattrvars,pattrvarstail,ignore_negative);
    }
    // if this is a flora formula, no need to check the last argument
    if (is_flora_form((prolog_term)pterm,ignore_negative))
      return;

    pterm = clref_val(pterm)+arity;
    goto groundBegin;

  default:
    xsb_abort("[FLORA]: BUG in flrterm_vars_split/3: term with unknown tag (%d)",
	      (int)cell_tag(pterm));
    return;
  }
}



/***** auxiliary *******/

static inline xsbBool is_scalar(prolog_term pterm)
{
  if (is_atom(pterm) || is_int(pterm) || is_float(pterm))
    return TRUE;
  return FALSE;
}


/* Check if pterm represents a formula rather than a term */
/* If ignore_negative=1, FL_TRUTHVALUE_TABLED_CALL and FL_TABLED_NAF_CALL
   are considered flora formulas. */
static inline int is_flora_form(prolog_term pterm, Integer ignore_negative)
{
  char *functor;
  int has_flora_prefix;

  if (is_scalar(pterm) || is_list(pterm)) return FALSE;

  functor = extern_p2c_functor(pterm);
  has_flora_prefix =
    (strncmp(functor, ERGO_META_PREFIX, ERGO_META_PREFIX_LEN)==0
     ||
     strncmp(functor,FLORA_META_PREFIX,FLORA_META_PREFIX_LEN)==0);

  if (has_flora_prefix && strstr(functor,FL_UNDEFEATED))
    return FALSE;
  if (!ignore_negative && has_flora_prefix &&
      (strstr(functor,FL_TRUTHVALUE_TABLED_CALL)
       || strstr(functor,FL_TABLED_NAF_CALL)
       ))
      return FALSE;

  return (has_flora_prefix);
}


/* Check if pterm represents a formula rather than a term */
static inline int is_flora_tnot_predicate(prolog_term pterm)
{
  char *functor;
  functor = extern_p2c_functor(pterm);
  return (strncmp(functor, FLORA_NAF_PREDICATE, FLORA_NAF_LEN)==0);
}

static int is_ignorable_functor(prolog_term pterm)
{
  char *functor;
  functor = extern_p2c_functor(pterm);
  return (strncmp(functor, "flora_put_attr", 14)==0);
}


/* assumes compound arity > 0 */
static inline prolog_term trim_compound(prolog_term pterm, int arity)
{
  prolog_term trimmed=extern_p2p_new();
  int j;

  extern_c2p_functor(extern_p2c_functor(pterm), arity-1, trimmed);
  for (j=1; j < arity ; j++)
    extern_p2p_unify(extern_p2p_arg(pterm,j), extern_p2p_arg(trimmed,j));

  return trimmed;
}

/* assumes list is non-nil */
static inline prolog_term trim_list(prolog_term inList)
{
  prolog_term inListHead, inListTail;
  prolog_term trimmedList=extern_p2p_new(), trimmedHead, trimmedTail;


  inListTail = inList;
  trimmedTail = trimmedList;

  while (!is_nil(inListTail)) {
    if (is_list(inListTail)) {
      inListHead = extern_p2p_car(inListTail);
      inListTail = extern_p2p_cdr(inListTail);
      if (!is_nil(inListTail)) {
	extern_c2p_list(trimmedTail);
	trimmedHead = extern_p2p_car(trimmedTail);
	extern_p2p_unify(trimmedHead, inListHead);
	trimmedTail = extern_p2p_cdr(trimmedTail);
      }
    }
    else
      break;
  }

  extern_c2p_nil(trimmedTail); /* bind trimmed tail to nil */
  
  return trimmedList;
}

#ifdef FG_DEBUG
static char *pterm2string(CTXTdeclc prolog_term term)
{ 
  prolog_term term2 = extern_p2p_deref(term);
  return extern_print_pterm_fun(term2); 
} 
#endif


// ask Terry what to do in the multi-threaded case here
#ifndef MULTI_THREAD
CTptr cycle_trail = 0;
int cycle_trail_size = 0;
int cycle_trail_top = -1;
#endif

// modified from the XSB code. We need to take care of not counting the last
// argument in FLORA-2 wrapper predicates
int local_ground_cyc(CTXTdeclc Cell Term, int cycle_action) { 
  Cell visited_string;

  XSB_Deref(Term);
  if (cell_tag(Term) != XSB_LIST && cell_tag(Term) != XSB_STRUCT) {
    if (!isnonvar(Term) || is_attv(Term) )
      return FALSE;
    else
      return TRUE;
  }

  if (cycle_trail == (CTptr) 0) {
    cycle_trail_size = TERM_TRAVERSAL_STACK_INIT;
    cycle_trail =
      (CTptr) mem_alloc(cycle_trail_size*sizeof(Cycle_Trail_Frame),OTHER_SPACE);
  }
  visited_string = makestring(string_find("_$visited",1));

  push_cycle_trail(Term);	
  *clref_val(Term) = visited_string;

  while (cycle_trail_top >= 0) {
#ifdef FG_DEBUG
    fprintf(stderr,"flrground_cyc: cycle_trail_top=%d\n", cycle_trail_top);
#endif

    if (cycle_trail[cycle_trail_top].arg_num > cycle_trail[cycle_trail_top].arity) {
      pop_cycle_trail(Term);	
#ifdef FG_DEBUG
      fprintf(stderr,"flrground_cyc0: pterm=%s\n", pterm2string(CTXTc Term));
#endif
    }
    else {
      if (cycle_trail[cycle_trail_top].arg_num == 0) {
	Term = cycle_trail[cycle_trail_top].value;
#ifdef FG_DEBUG
	fprintf(stderr,"flrground_cyc0.5: pterm=%s top=%d\n",pterm2string(CTXTc Term), cycle_trail_top);
#endif
      }
      else {
	 //printf("examining struct %p %d\n",clref_val(cycle_trail[cycle_trail_top].parent),cycle_trail[cycle_trail_top].arg_num);
	Term = (Cell) (clref_val(cycle_trail[cycle_trail_top].parent)
		       + cycle_trail[cycle_trail_top].arg_num);
#ifdef FG_DEBUG
	fprintf(stderr,"flrground_cyc1: pterm=%s\n", pterm2string(CTXTc Term));
#endif
      }
      cycle_trail[cycle_trail_top].arg_num++;
#ifdef FG_DEBUG
      fprintf(stderr,"flrground_cyc11: pterm=%s  %d\n", pterm2string(CTXTc Term), cycle_trail[cycle_trail_top].arg_num);
#endif
      // printf("Term1 before %p\n",Term);
      XSB_Deref(Term);
      //printf("Term1 after %p\n",Term);
#ifdef FG_DEBUG
      fprintf(stderr,"flrground_cyc2: pterm=%s\n", pterm2string(CTXTc Term));
#endif
      if (cell_tag(Term) != XSB_LIST && cell_tag(Term) != XSB_STRUCT) {
#ifdef FG_DEBUG
	fprintf(stderr,"flrground_cyc3: pterm=%s\n", pterm2string(CTXTc Term));
#endif
        if (!isnonvar(Term) || is_attv(Term) ) { 
	  unwind_cycle_trail;
	  return FALSE; 
        }   
       }
       else {
	// printf("*clref_val(TERM) %d\n",*clref_val(Term));
	if (*clref_val(Term) == visited_string) {
           // printf("unwind_cycle_trail\n");
	  unwind_cycle_trail;
	  // cycle found
	  if (cycle_action == CYCLIC_SUCCEED) 
	    return TRUE;
	  else return FALSE;
	}
	else {
	  // printf("push_cycle_trail\n");
#ifdef FG_DEBUG
	  fprintf(stderr,"flrground_cyc4: pterm=%s\n",pterm2string(CTXTc Term));
#endif
	  push_cycle_trail(Term);	
#ifdef FG_DEBUG
	  fprintf(stderr,"flrground_cyc5: pterm=%s\n",pterm2string(CTXTc Term));
#endif
	  // printf("assign *clref_val(Term)\n");
	  *clref_val(Term) = visited_string;
#ifdef FG_DEBUG
	  fprintf(stderr,"flrground_cyc6: pterm=%s\n",pterm2string(CTXTc Term));
#endif
	}
       }
    }
  }
  return TRUE;
}


/*
** Trim white space from string's (Arg1) ends. Return trimmed string in Arg2.
** Arg3: 0 - trim from both ends; >0 - trim from left end; <0 - from right end
*/
DllExport xsbBool call_conv flrtrim_whitespace (CTXTdecl)
{
  char *instring = ptoc_string(CTXTc 1);
  prolog_term outstring = extern_reg_term(2);
  Integer direction = ptoc_int(CTXTc 3);

  char *tempstr;
  Cell interned_result;
  size_t tempstr_len = strlen(instring);
  size_t lowpos=0, highpos=tempstr_len;

  tempstr = (char *)mem_alloc(tempstr_len+1,LEAK_SPACE);
  strcpy(tempstr, instring);

  if (direction >= 0)  {
    //fprintf(stderr,"lowpos=%d\n",lowpos);
    // trim from the left
    while (isspace(*(tempstr+lowpos)) && lowpos < highpos) {
      lowpos++;
      //fprintf(stderr,"lowpos=%d\n",lowpos);
    }
  }
  if (direction <= 0) {
    //fprintf(stderr,"highpos=%d\n",highpos);
    // trim from the right
    highpos--;
    while (isspace(*(tempstr+highpos)) && lowpos < highpos) {
      highpos--;
      //fprintf(stderr,"highpos=%d\n",highpos);
    }
  }

  *(tempstr+highpos+1) = '\0';
  interned_result = makestring(string_find(tempstr+lowpos,1));
  mem_dealloc(tempstr,tempstr_len,LEAK_SPACE);

  extern_p2p_unify(interned_result,outstring);
  return TRUE;
}
