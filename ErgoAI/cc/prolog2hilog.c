/* File:      prolog2hilog.c
** Author(s): Michael Kifer
**
** Contact:   see  ../CONTACTS.txt
**
** Copyright (C) by
**      The Research Foundation of the State University of New York, 1999-2018.
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



#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#endif



#include "xsb_config.h"

#include <stdio.h>
#include <string.h>

#ifdef WIN_NT
#define XSB_DLL
#endif

#include "auxlry.h"
#include "context.h"
#include "cell_xsb.h"
#include "error_xsb.h"
#include "cinterf.h"
#include "tries.h"
#include "wind2unix.h"
#include "debug_xsb.h"

#include "flora_cc_prefix.h"

#define FLORA_LIB_PREFIX          "fllib"
#define FLORA_DATATYPE_FUNC       "\\datatype"
#define FLORA_LIB_PREFIX_LEN      5
#define FLORA_DATATYPE_FUNC_LEN   7


#if 0
#define P2HDEBUG
#define P2HDEBUG_VERBOSE
#endif

/* take hilog term and a hilog apply op and return prolog term.
   If the apply term is != the one used in the hilog term, assume it is already
   a prolog term and don't convert */
static prolog_term hilog2prolog(CTXTdeclc prolog_term hterm, char *apply, Integer unify_vars);
/* take prolog term and a symbol name of the apply operator and return hilog
   term. If prolog term already has the main functor==hilog apply, then don't
   convert. */
static prolog_term prolog2hilog(CTXTdeclc prolog_term pterm, char *apply, Integer unify_vars);
static char *pterm2string(CTXTdeclc prolog_term term);
inline static int is_hilog(prolog_term term, char *apply_funct);
inline static int is_special_form_binary(prolog_term term);
inline static int is_special_form_unary(prolog_term term);
inline static int is_formula(prolog_term term);
inline static int is_protected_term(prolog_term term);
static prolog_term map_special_form_binary(CTXTdeclc prolog_term (*func)(), prolog_term term, char *apply, Integer unify_vars);
static prolog_term map_special_form_unary(CTXTdeclc prolog_term (*func)(), prolog_term term, char *apply, Integer unify_vars);
static prolog_term map_list(CTXTdeclc prolog_term func(), prolog_term term, char *apply, Integer unify_vars);

#define MAX_ERROR_MSGLEN  4000
static char errormessage[MAX_ERROR_MSGLEN+1];

/*
  When called from Prolog, takes 3 args:
  - Pterm:  Prolog term
  - Hterm:  HiLog term
  - Apply:  Symbol name for the HiLog apply predicate
  - UnifyFlag: If true, unify if both Pterm and Hterm are variables

  If Pterm is a variable, then it is unified with Hterm.
  If Hterm is a variable, then it is unified with Pterm.
  If both Pterm and Hterm are scalar (int, float, string), then they are
  unified. 

  If Pterm or Hterm is a list or a commalist (a,b,c,), then the function is
  applied to each element and the results are returned as a list or a commalist
  (whichever applies).

  If Hterm is term (and not a list or a commalist), then Hterm is assumed to be
  a HiLog term of the kind that converts to Prolog. Hterm is then converted to
  Prolog and the result is unified with Pterm.  If the main functor is !=
  Apply, then it is assumed to be a prolog term and the term is returned
  without conversion.

  If Pterm is a term that is not a list or a commalist, then it is assumed to
  be a prolog term. It is converted to HiLog using the apply-functor name given
  in Apply (which must be an atom).  The result then unifies with Hterm.  If
  the main functor is = Apply, then we assume that the term is already a HiLog
  term and the term is simply returned without change.

  For instance,
      flora_plg2hlg(f(a,g(b,X)),Y,abc)
      Y = abc(f,a,abc(g,b,_h123))

      flora_plg2hlg(X, cde(f,a,cde(g,b,Y))),Z)
      X = abc(f,a,abc(g,b,_h123))
      Z = abc

  Doesn't do occur-check!!! Something like
      flora_plg2hlg(X, cde(f,a,cde(g,b,X))),Z)
  Will loop and eventually crash because X occurs in Pterm and in Hterm.
 */
DllExport xsbBool call_conv flora_plg2hlg (CTXTdecl) {
  prolog_term pterm = extern_reg_term(1);
  prolog_term hterm = extern_reg_term(2);
  prolog_term apply_t = extern_reg_term(3);
  Integer unify_vars = extern_ptoc_int(4); /* whether to unify if both args are vars */
  prolog_term temp_term;
  char *apply;

#ifdef P2HDEBUG_VERBOSE
  fprintf(stderr,"flora_plg2hlg: Arg1=%s\n", pterm2string(CTXTc pterm));
  fprintf(stderr,"flora_plg2hlg: Arg2=%s\n", pterm2string(CTXTc hterm));
  fprintf(stderr,"flora_plg2hlg: Arg3=%s\n", pterm2string(CTXTc apply_t));
#endif

#ifdef P2HDEBUG
  if (!is_atom(apply_t)) {
    snprintf(errormessage, MAX_ERROR_MSGLEN,
	    "p2h{?Plg,?Hlg}: The apply functor=%s is not an atom.",
	    pterm2string(CTXTc apply_t));
    xsb_abort(errormessage);
  }
#endif

  apply = string_val(apply_t);

  /* both are variables */
  if (is_var(pterm) && is_var(hterm)) {
    if (unify_vars)
      return extern_p2p_unify(pterm,hterm);
    else
      return TRUE;
  }

  /* if hilog is instantiated, convert from hilog to prolog
     and unify, because hilog->prolog conversion is more accurate */
  if (!is_var(hterm)) {
    temp_term = hilog2prolog(CTXTc hterm, apply,unify_vars);
    return extern_p2p_unify(temp_term, pterm);
  }

  /* hterm is a variable and pterm is not */
  temp_term = prolog2hilog(CTXTc pterm, apply, unify_vars);

  return extern_p2p_unify(temp_term, hterm);
}

static inline xsbBool is_scalar(prolog_term pterm)
{
  if (is_atom(pterm) || is_int(pterm) || is_float(pterm))
    return TRUE;
  return FALSE;
}


static prolog_term hilog2prolog(CTXTdeclc prolog_term hterm, char *apply, Integer unify_vars)
{
  prolog_term pterm = extern_p2p_new();
  prolog_term pfunctor;
  int arity, i;

  if (is_var(hterm)){
    return hterm;
  }
  if (is_scalar(hterm)) return hterm;
  if (is_protected_term(hterm)) return hterm;

  if (is_list(hterm))
    return map_list(CTXTc hilog2prolog,hterm,apply,unify_vars);
  else if (is_special_form_binary(hterm))
    return map_special_form_binary(CTXTc hilog2prolog,hterm,apply,unify_vars);
  else if (is_special_form_unary(hterm))
    return map_special_form_unary(CTXTc hilog2prolog,hterm,apply,unify_vars);

#ifdef P2HDEBUG
  if (!is_functor(hterm)) {
    snprintf(errormessage, MAX_ERROR_MSGLEN,
	    "p2h{?Plg,?Hlg}: ?Hlg=%s must be a HiLog term.",
	    pterm2string(CTXTc hterm));
    xsb_abort(errormessage);
  }
#endif

  /* Don't convert if already Prolog */
  if (!is_hilog(hterm,apply)) return hterm;
  /* Don't convert if formula (predicate or molecule) */
  if (is_formula(hterm)) return hterm;

  arity=extern_p2c_arity(hterm);

  pfunctor = extern_p2p_arg(hterm,1);
  if (!is_atom(pfunctor)) {
    snprintf(errormessage, MAX_ERROR_MSGLEN,
	    "p2h{?Plg,?Hlg}: ?Hlg=%s is not convertible to Prolog.",
	    pterm2string(CTXTc hterm));
    xsb_abort(errormessage);
  }
  if (arity > 1)
    extern_c2p_functor(string_val(pfunctor), arity-1, pterm);
  else
    return pfunctor;

#ifdef P2HDEBUG_VERBOSE
  fprintf(stderr,"h2p start: Pterm=%s", pterm2string(CTXTc pterm));
  fprintf(stderr,"h2p start: Hterm=%s", pterm2string(CTXTc hterm));
  fprintf(stderr,"h2p start: Apply=%s", apply);
#endif

  for (i=2; i<=arity; i++) {
    extern_p2p_unify(hilog2prolog(CTXTc extern_p2p_arg(hterm,i),apply,unify_vars),
		     extern_p2p_arg(pterm, i-1));
#ifdef P2HDEBUG_VERBOSE
    fprintf(stderr,"h2p loop: Pterm=%s\n", pterm2string(CTXTc pterm));
#endif
  }
  return pterm;
}


static prolog_term prolog2hilog(CTXTdeclc prolog_term pterm, char *apply, Integer unify_vars)
{
  prolog_term hterm = extern_p2p_new();
  int arity, i;

  if (is_var(pterm)) {
    return pterm;
  }
  if (is_scalar(pterm)) return pterm;
  if (is_protected_term(pterm)) return pterm;

  if (is_list(pterm))
    return map_list(CTXTc prolog2hilog,pterm,apply, unify_vars);
  else if (is_special_form_binary(pterm))
    return map_special_form_binary(CTXTc prolog2hilog,pterm,apply, unify_vars);
  else if (is_special_form_unary(pterm))
    return map_special_form_unary(CTXTc prolog2hilog,pterm,apply, unify_vars);

  if (!is_functor(pterm)) {
    snprintf(errormessage, MAX_ERROR_MSGLEN,
	    "p2h{?Plg,?Hlg}: ?Plg=%s must be a Prolog term.",
	    pterm2string(CTXTc pterm));
    xsb_abort(errormessage);
  }

  /* Don't convert if already HiLog */
  if (is_hilog(pterm,apply)) return pterm;
  /* Don't convert if formula (predicate or molecule) */
  if (is_formula(pterm)) return pterm;

  arity = extern_p2c_arity(pterm);
  extern_c2p_functor(apply,arity+1,hterm);
  extern_c2p_string(extern_p2c_functor(pterm), extern_p2p_arg(hterm,1)); /* set the functor arg */

#ifdef P2HDEBUG_VERBOSE
  fprintf(stderr,"p2h start: Pterm=%s\n", pterm2string(CTXTc pterm));
  fprintf(stderr,"p2h start: Hterm=%s\n", pterm2string(CTXTc hterm));
  fprintf(stderr,"p2h start: Apply=%s\n", apply);
#endif

  /* set the rest of the args */
  for (i=1; i<=arity; i++) {
    extern_p2p_unify(prolog2hilog(CTXTc extern_p2p_arg(pterm,i),apply, unify_vars), extern_p2p_arg(hterm,i+1));
#ifdef P2HDEBUG_VERBOSE
    fprintf(stderr,"p2h loop: Hterm=%s\n", pterm2string(CTXTc hterm));
#endif
  }
  return hterm;
}


static prolog_term map_list(CTXTdeclc prolog_term func(), prolog_term termList, char *apply, Integer unify_vars)
{
  prolog_term listHead, listTail;
  prolog_term outList=extern_p2p_new(), outListHead, outListTail;
  prolog_term temp_term;
  xsbBool mustExit=FALSE;


  listTail = termList;
  outListTail = outList;

  while (!is_nil(listTail) && !mustExit) {
    if (is_list(listTail)) {
      extern_c2p_list(outListTail);
      listHead = extern_p2p_car(listTail);
      outListHead = extern_p2p_car(outListTail);
      temp_term = func(CTXTc listHead,apply, unify_vars);
      extern_p2p_unify(outListHead, temp_term);
      listTail = extern_p2p_cdr(listTail);
      outListTail = extern_p2p_cdr(outListTail);
    } else {
      extern_p2p_unify(listTail,outListTail);
      mustExit = TRUE;
    }
  }

 if (is_nil(listTail)) 
   extern_c2p_nil(outListTail); /* bind tail to nil */
  
  return outList;
}

static prolog_term map_special_form_binary(CTXTdeclc prolog_term (*func)(), prolog_term special_form, char *apply, Integer unify_vars)
{
  prolog_term formArg1_temp, formArg2_temp;
  prolog_term out_form=extern_p2p_new(), formArg1_out, formArg2_out;
  char *functor = extern_p2c_functor(special_form);

#ifdef P2HDEBUG_VERBOSE
  fprintf(stderr,"in map_special_form_binary: spec_form=%s\n", pterm2string(CTXTc special_form));
#endif

  extern_c2p_functor(functor, 2, out_form);
  formArg1_out = extern_p2p_arg(out_form,1);
  formArg2_out = extern_p2p_arg(out_form,2);
  formArg1_temp = func(CTXTc extern_p2p_arg(special_form,1),apply,unify_vars);
  formArg2_temp = func(CTXTc extern_p2p_arg(special_form,2),apply,unify_vars);
  extern_p2p_unify(formArg1_out,formArg1_temp);
  extern_p2p_unify(formArg2_out,formArg2_temp);

  return out_form;
}


static prolog_term map_special_form_unary(CTXTdeclc prolog_term (*func)(), prolog_term special_form, char *apply, Integer unify_vars)
{
  prolog_term formArg1_temp;
  prolog_term out_form=extern_p2p_new(), formArg1_out;
  char *functor = extern_p2c_functor(special_form);

#ifdef P2HDEBUG_VERBOSE
  fprintf(stderr,"in map_special_form_unary: spec_form=%s\n", pterm2string(CTXTc special_form));
#endif

  extern_c2p_functor(functor, 1, out_form);
  formArg1_out = extern_p2p_arg(out_form,1);
  formArg1_temp = func(CTXTc extern_p2p_arg(special_form,1),apply,unify_vars);
  extern_p2p_unify(formArg1_out,formArg1_temp);

  return out_form;
}



static char *pterm2string(CTXTdeclc prolog_term term)
{ 
  prolog_term term2 = extern_p2p_deref(term);
  return extern_print_pterm_fun(term2); 
} 


/* This detects both HiLog terms and predicates, but we really need to check
   for HiLog terms only */
static int is_hilog(prolog_term term, char *apply_funct)
{
  char *func = extern_p2c_functor(term); /* term functor */
  int length_diff = (int) strlen(func) - (int) strlen(apply_funct);
  //size_t length_diff = strlen(func) - strlen(apply_funct);
  
  if (0 > length_diff) return FALSE;

  /* Match apply_funct to the end of the term functor.
     HiLog terms have functor=apply_functor.
     HiLog predicates have complex functor, whose tail matches WRAP_HILOG */
  return (strcmp(apply_funct, func+length_diff)==0);
}


/* Check if term represents a formula rather than a term */
static int is_formula(prolog_term term)
{
  char *functor;
  if (is_scalar(term) || is_list(term)) return FALSE;

  functor = extern_p2c_functor(term);
  return
    (strncmp(functor, ERGO_META_PREFIX, ERGO_META_PREFIX_LEN)==0)
    ||
    (strncmp(functor, FLORA_META_PREFIX, FLORA_META_PREFIX_LEN)==0)
    ||
    (strncmp(functor, FLORA_LIB_PREFIX, FLORA_LIB_PREFIX_LEN)==0);
}


/* Check if term is protected from conversion. Example: a datatype, = */
static int is_protected_term(prolog_term term)
{
  char *functor;
  int arity;

  if (is_scalar(term) || is_list(term)) return FALSE;

  // must be AFTER the above checks: crashes on lists
  arity =extern_p2c_arity(term);

  functor = extern_p2c_functor(term);

  if (arity == 1 &&
      (strcmp(functor,"NULL") == 0
       // ||   // NOTE: subterms won't be converted!
       ))
      return TRUE;

  return
    (
     strncmp(functor, FLORA_DATATYPE_FUNC, FLORA_DATATYPE_FUNC_LEN)==0
     // ||    // add more as needed. NOTE: subterms won't be converted!
     );
}


/* Note: this only treats 2-ary 1-character and some 2-character functors
   that are treated as prolog in Flora.
   We don't do it for others due to speed considerations. */
static int is_special_form_binary(prolog_term term)
{
  char *functor;
  int arity;

  if (is_scalar(term) || is_list(term)) return FALSE;
  // must be AFTER the above checks: crashes on lists
  arity =extern_p2c_arity(term);

  if (arity != 2) return FALSE;

  functor = extern_p2c_functor(term);
  if (
      strcmp(functor,":-") == 0
      || strcmp(functor,"==") == 0
      || strcmp(functor,"\\=") == 0
      || strcmp(functor,"=<") == 0
      || strcmp(functor,">=") == 0
      )
    return TRUE; // rules are also protected

  if (strlen(functor)==1) {
    switch (*functor) {
    case ',':
    case ';':
    case '+':
    case '-':
    case '/':
    case '*':
    case '>':
    case '<':
    case '=':
    case '~': return TRUE;
    default: return FALSE;
    }
  }
  return FALSE;
}

// some unary, 1-char functors
static int is_special_form_unary(prolog_term term)
{
  char *functor;
  int arity;

  if (is_scalar(term) || is_list(term)) return FALSE;
  // must be AFTER the above checks: crashes on lists
  arity =extern_p2c_arity(term);

  if (arity != 1) return FALSE;

  functor = extern_p2c_functor(term);

  if (strlen(functor)==1) {
    switch (*functor) {
    case '+':
    case '-': return TRUE;
    default: return FALSE;
    }
  }
  return FALSE;
}



/* 
   flora_plg2hlg(a(qq,b(c,4),b(c,5,d(X,U))),Y,aaa,1).
     Y = aaa(a,qq,aaa(b,c,4),aaa(b,c,5,aaa(d,_h312,_h313)))
   flora_plg2hlg(aaa(qq,b(c,4)),X,aaa,1).
     X = aaa(qq,b(c,4))
   flora_plg2hlg(X, aaa(qq,b(c,4),aaa(kkk,Bbb,aaa(ppp,aaa(uuu,Aaa),Ooo))),aaa,1).
     X = qq(b(c,4),kkk(_h356,ppp(uuu(_h365),_h362)))
   flora_plg2hlg(X, aaa(qq,aaa(aaa,4)),aaa,1).
     X = qq(aaa(4))
   flora_plg2hlg(X, [], aaa,1).
     X = []
   flora_plg2hlg([], X, aaa,1).
     X = []
   flora_plg2hlg(X, [aaa(qq,b(c,4)), f(abc), aaa(b,c(K),aaa(bbb,aaa(ccc,aaa(ddd))))],aaa,1).
     X = [qq(b(c,4)),f(abc),b(c(_h185),bbb(ccc(ddd)))]
   flora_plg2hlg(X, [aaa(qq,b(c,4)), f(abc), aaa(b,c(K))],aaa,1).
     X = [qq(b(c,4)),f(abc),b(c(_h185))]
   flora_plg2hlg(X, [[aaa(qq,b(c,4)), f(abc)], aaa(b,c(K))],aaa,1).
     X = [[qq(b(c,4)),f(abc)],b(c(_h193))]
   flora_plg2hlg([aaa(qq,b(c,4)), a(qq,b(c,4)), f(q(a),b,c(p,q(Y)))], X, aaa,1).
     X = [aaa(qq,b(c,4)),aaa(a,qq,aaa(b,c,4)),aaa(f,aaa(q,a),b,aaa(c,p,aaa(q,_h423)))]
   flora_plg2hlg([aaa(qq,b(c,4)), [a(qq,b(c,4))], [f(q(a),b,c(p,q(Y))), b(_)]], X, aaa,1).
     X = [aaa(qq,b(c,4)),[aaa(a,qq,aaa(b,c,4))],[aaa(f,aaa(q,a),b,aaa(c,p,aaa(q,_h480))),aaa(b,_h487)]]
   flora_plg2hlg(X, (aaa(qq,b(c,4)), f(abc), aaa(b,c(K),aaa(bbb,aaa(ccc,aaa(ddd))))),aaa,1).
     X = (qq(b(c,4))  ','  f(abc)  ','  b(c(_h185),bbb(ccc(ddd))))
   flora_plg2hlg(X, (aaa(qq,b(c,4)), f(abc), aaa(b,c(K))),aaa,1).
     X = (qq(b(c,4))  ','  f(abc)  ','  b(c(_h185)))
   flora_plg2hlg(X, ((aaa(qq,b(c,4)); f(abc)), aaa(b,c(K))),aaa,1).
     X = ((qq(b(c,4))  ';'  f(abc))  ','  b(c(_h193)))
   flora_plg2hlg((aaa(qq,b(c,4)); a(qq,b(c,4)), f(q(a),b,c(p,q(Y)))), X,aaa,1).
     X = (aaa(qq,b(c,4))  ';'  aaa(a,qq,aaa(b,c,4))  ','  aaa(f,aaa(q,a),b,aaa(c,p,aaa(q,_h427))))
   flora_plg2hlg((aaa(qq,b(c,4)), a(qq,b(c,4)), f(q(a),b,c(p,q(Y)))), X,aaa,1).
     X = (aaa(qq,b(c,4))  ','  aaa(a,qq,aaa(b,c,4))  ','  aaa(f,aaa(q,a),b,aaa(c,p,aaa(q,_h427))))
   flora_plg2hlg(((aaa(qq,b(c,4)), a(qq,b(c,4))); (f(q(a),b,c(p,q(Y))), b(_))), X, aaa,1).
     X= (aaa(qq,b(c,4))  ','  aaa(a,qq,aaa(b,c,4))  ';' aaa(f,aaa(q,a),b,aaa(c,p,aaa(q,_h480)))  ','  aaa(b,_h485))
*/
