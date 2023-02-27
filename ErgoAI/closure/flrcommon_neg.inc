/*******************************************************************************
  file: closure/flrcommon_neg.inc - A common part of every trailer

  Author(s): Michael Kifer

  Contact:   see  ../CONTACTS.txt

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

/***************************************************************************
  inferred predicates for explicit negation, neg

  These wrapper predicates with the 'inferred_' prefix (FLNEGTRAILERPREFIX)
  are needed to make the trailer dynamically loadable. Note that the
  program rules use wrapper predicates with the 'derived_', prefix, i.e.,
  FLNEGHEADPREFIX.
***************************************************************************/
FLNEGTRAILERPREFIX(WRAP_SUB)(X,Y,FWContext) :-
        %% \neg ::'s meaning is fixed for builtin types in flrdtype_sub.P
        X \== FL_TOPTYPE,
        Y \== FL_TOPTYPE,
        \+ flrdatatype:flora_builtin_datatype(X),
        \+ flrdatatype:flora_builtin_datatype(Y),
	FLNEGHEADPREFIX(WRAP_SUB)(X,Y,FWContext).
FLNEGTRAILERPREFIX(WRAP_ISA)(O,C,FWContext) :-
        %% \neg :'s meaning is fixed for builtin types in flrdtype_sub.P
        C \== FL_TOPTYPE,
        \+ flrdatatype:flora_builtin_datatype(C),
	FLNEGHEADPREFIX(WRAP_ISA)(O,C,FWContext).
FLNEGTRAILERPREFIX(WRAP_MVD)(O,A,V,FWContext) :-
	FLNEGHEADPREFIX(WRAP_MVD)(O,A,V,FWContext).
FLNEGTRAILERPREFIX(WRAP_IMVD)(C,A,V,FWContext) :-
	FLNEGHEADPREFIX(WRAP_IMVD)(C,A,V,FWContext).
FLNEGTRAILERPREFIX(WRAP_MVDDEF)(O,A,FWContext) :-
	FLNEGHEADPREFIX(WRAP_MVDDEF)(O,A,FWContext).
FLNEGTRAILERPREFIX(WRAP_IMVDDEF)(C,A,FWContext) :-
	FLNEGHEADPREFIX(WRAP_IMVDDEF)(C,A,FWContext).
FLNEGTRAILERPREFIX(WRAP_MVDSIG)(O,MethodArgs,R,FWContext) :-
	FLNEGHEADPREFIX(WRAP_MVDSIG)(O,MethodArgs,R,FWContext).
FLNEGTRAILERPREFIX(WRAP_MVDSIGDEF)(O,MethodArgs,FWContext) :-
	FLNEGHEADPREFIX(WRAP_MVDSIGDEF)(O,MethodArgs,FWContext).
FLNEGTRAILERPREFIX(WRAP_IMVDSIG)(O,MethodArgs,R,FWContext) :-
	FLNEGHEADPREFIX(WRAP_IMVDSIG)(O,MethodArgs,R,FWContext).
FLNEGTRAILERPREFIX(WRAP_IMVDSIGDEF)(O,MethodArgs,FWContext) :-
	FLNEGHEADPREFIX(WRAP_IMVDSIGDEF)(O,MethodArgs,FWContext).
FLNEGTRAILERPREFIX(WRAP_METH)(O,M,FWContext) :-
	FLNEGHEADPREFIX(WRAP_METH)(O,M,FWContext).
FLNEGTRAILERPREFIX(WRAP_IMETH)(C,M,FWContext) :-
	FLNEGHEADPREFIX(WRAP_IMETH)(C,M,FWContext).
FLNEGTRAILERPREFIX(WRAP_BOOLSIG)(O,M,FWContext) :-
	FLNEGHEADPREFIX(WRAP_BOOLSIG)(O,M,FWContext).
FLNEGTRAILERPREFIX(WRAP_IBOOLSIG)(C,M,FWContext) :-
	FLNEGHEADPREFIX(WRAP_IBOOLSIG)(C,M,FWContext).
FLNEGTRAILERPREFIX(WRAP_OBJEQL)(X,Y,FWContext) :-
	FLNEGHEADPREFIX(WRAP_OBJEQL)(X,Y,FWContext).
FLNEGTRAILERPREFIX(WRAP_EXISTS)(X,FWContext) :-
	FLNEGHEADPREFIX(WRAP_EXISTS)(X,FWContext).
