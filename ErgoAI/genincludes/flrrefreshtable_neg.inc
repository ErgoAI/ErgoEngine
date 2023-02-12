/*******************************************************************************
  file: genincludes/flrrefreshtable_neg.inc - A template for commands for
					      deleting facts from Prolog tables.
  Author(s): Michael Kifer

  Copyright (C) by
       The Research Foundation of the State University of New York, 1999-2018;
       and Vulcan, Inc., 2008-2013.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*******************************************************************************/

:- dynamic FLORA_SYMBOL('refresh_table_for_neg_loaded')/1.

%% ISA, SUB
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_SUB)(X,Y,_FWContext)) :-
	    	    !,
	       	    flora_abolish_table_call(WRAP_SUB(X,Y,_)),
	       	    flora_abolish_table_call(FLNEGPREFIX(WRAP_SUB)(X,Y,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
            FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_ISA)(X,Y,_)) :-
	    	    !,
		    flora_abolish_table_call(WRAP_ISA(X,Y,_)),
		    flora_abolish_table_call(FLNEGPREFIX(WRAP_ISA)(X,Y,_))
	  )).
	
%% MVD, IMVD
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_MVD)(X,Y,Z,_FWContext)) :-
	    	    !,
		    flora_abolish_table_call(WRAP_MVD(X,Y,Z,_)),
		    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVD)(X,Y,Z,_)),
	    	    flora_abolish_table_call(WRAP_MVDDEF(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVDDEF)(X,Y,_))
	  )).
	
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_IMVD)(X,Y,Z,_FWContext))
    	    :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_IMVD(X,Y,Z,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_IMVD)(X,Y,Z,_)),
	    	    flora_abolish_table_call(WRAP_IMVDDEF(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_IMVDDEF)(X,Y,_)),
	    	    flora_abolish_table_call(WRAP_MVD(X,Y,Z,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVD)(X,Y,Z,_)),
	    	    flora_abolish_table_call(WRAP_MVDDEF(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVDDEF)(X,Y,_))
	  )).

%% MVDSIG, IMVDSIG
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_MVDSIG)(X,Y,Z,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_MVDSIG(X,Y,Z,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVDSIG)(X,Y,Z,_))
	  )).
	
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_IMVDSIG)(X,Y,Z,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_IMVDSIG(X,Y,Z,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_IMVDSIG)(X,Y,Z,_)),
	    	    flora_abolish_table_call(WRAP_MVDSIG(X,Y,Z,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVDSIG)(X,Y,Z,_))
	  )).

%% MVDDEF, IMVDDEF

?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_MVDDEF)(X,Y,_FWContext)) :-
	    	    !,
		    flora_abolish_table_call(WRAP_MVDDEF(X,Y,_)),
		    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVDDEF)(X,Y,_))
	  )).
	
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_IMVDDEF)(X,Y,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_IMVDDEF(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_IMVDDEF)(X,Y,_)),
	    	    flora_abolish_table_call(WRAP_MVDDEF(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_MVDDEF)(X,Y,_))
	  )).

%% METH, IMETH
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_METH)(X,Y,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_METH(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_METH)(X,Y,_))
	  )).
	
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_IMETH)(X,Y,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_IMETH(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_IMETH)(X,Y,_)),
	    	    flora_abolish_table_call(WRAP_METH(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_METH)(X,Y,_))
	  )).

%% BOOLSIG, IBOOLSIG
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_BOOLSIG)(X,Y,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_BOOLSIG(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_BOOLSIG)(X,Y,_))
	  )).
	
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_IBOOLSIG)(X,Y,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_IBOOLSIG(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_IBOOLSIG)(X,Y,_)),
	    	    flora_abolish_table_call(WRAP_BOOLSIG(X,Y,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_BOOLSIG)(X,Y,_))
	  )).

	
%% EXISTS, OBJEQL
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_EXISTS)(X,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_EXISTS(X,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_EXISTS)(X,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_OBJEQL)(X,Y,_FWContext)) :-
	    	    !,
	    	    flora_abolish_table_call(WRAP_DIFFOBJEQL(X,Y,_))
	  )).


%% HiLog, up to 21 arguments
/*
**    *** The rules below are just dummies - they don't work because it is
**        hard to efficiently convert a call of the form
***       WRAP_NONTABLED_HILOG(fun,args...)
**        to WRAP_TABLED_HILOG(rule-index,fun,args...)
**        When we switch to a different implementation of tabling we will
**        revisit these rules.
*/
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_))
	  )).
?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert((
    	    FLORA_SYMBOL('refresh_table_for')(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_FWContext))
	    :-
	            !,
	    	    flora_abolish_table_call(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_)),
	    	    flora_abolish_table_call(FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_))
	  )).


?- \+ FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME'),
   assert(FLORA_SYMBOL('refresh_table_for_neg_loaded')('FLORA_THIS_MODULE_NAME')).
