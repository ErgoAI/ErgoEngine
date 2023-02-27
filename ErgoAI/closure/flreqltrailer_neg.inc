/*******************************************************************************
  file: closure/flreqltrailer_neg.inc - A template for generating trailers with
  				      	equality maintenance and NO behavioral
					inheritance.
  					Inheritance of signatures is supported.
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
  Note that all programs rules are translated with with FLNEGHEADPREFIX.
  These rules are bridged to the trailer using wrapper predicates with
  FLNEGTRAILERPREFIX.
***************************************************************************/

%%%% THIS CONTAINS NEGATIVE AXIOMS THAT ARE GOOD BOTH FOR INHERITANCE AND
%%%% NO_INHERITANCE VERSIONS


/***************************************************************************
  closure rules for neg X::Y
***************************************************************************/
%% FLNEGTRAILERPREFIX(WRAP_SUB_EQL)/3
FLNEGTRAILERPREFIX(WRAP_SUB_EQL)(X,Y,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(X,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(Y,OY,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_SUB)(OX,OY,FL_CONTEXT(Caller,_,_)).

%% FLNEGPREFIX(WRAP_SUB)/3
FLNEGPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_SUB_EQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)).

%% X::Z, \neg X::Y |= \neg Z::Y
FLNEGPREFIX(WRAP_SUB)(Z,Y,FL_CONTEXT(Caller,_,_)) :-
	silent_sub(X,Z,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_SUB_EQL)(X,Y,FL_CONTEXT(Caller,_,_)).


%% \neg X::Z, Y::Z |= \neg X::Y
FLNEGPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,_,_)) :-
	silent_sub(Y,Z,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_SUB_EQL)(X,Z,FL_CONTEXT(Caller,_,_)).

%% X:Z, \neg X:Y |= \neg Z::Y
FLNEGPREFIX(WRAP_SUB)(Z,Y,FL_CONTEXT(Caller,_,_)) :-
        WRAP_ISA(X,Z,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_ISA_EQL)(X,Y,FL_CONTEXT(Caller,_,_)).



/***************************************************************************
  closure rules for  neg O:C
***************************************************************************/
%% FLNEGTRAILERPREFIX(WRAP_ISA_EQL)/3
FLNEGTRAILERPREFIX(WRAP_ISA_EQL)(O,C,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(C,CX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_ISA)(OX,CX,FL_CONTEXT(Caller,_,_)).


%% FLNEGPREFIX(WRAP_ISA)/3
FLNEGPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_ISA_EQL)(O,C,FL_CONTEXT(Caller,Id1,Id2)).

%% \neg O:C, S::C |= \neg O:S
FLNEGPREFIX(WRAP_ISA)(O,S,FL_CONTEXT(Caller,_,_)) :-
	silent_sub(S,C,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_ISA_EQL)(O,C,FL_CONTEXT(Caller,_,_)).


/*****************************************************************************
  The following rules propogate the effects of equality.
*****************************************************************************/
%% FLNEGTRAILERPREFIX(WRAP_IMVD_EQL)/4
FLNEGTRAILERPREFIX(WRAP_IMVD_EQL)(C,M,V,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(V,VX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_IMVD)(CX,MX,VX,FL_CONTEXT(Caller,_,_)).


%% FLNEGTRAILERPREFIX(WRAP_IMVDDEF_EQL)/3
FLNEGTRAILERPREFIX(WRAP_IMVDDEF_EQL)(C,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_IMVDDEF)(CX,MX,FL_CONTEXT(Caller,_,_)).


/***************************************************************************
  closure for X[Y->Z], X[Y*->Z], X[]
/***************************************************************************/
%% FLNEGTRAILERPREFIX(WRAP_MVD_EQL)/4
FLNEGTRAILERPREFIX(WRAP_MVD_EQL)(O,M,V,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(V,VX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_MVD)(OX,MX,VX,FL_CONTEXT(Caller,_,_)).


%% FLNEGTRAILERPREFIX(WRAP_MVDDEF_EQL)/3

FLNEGTRAILERPREFIX(WRAP_MVDDEF_EQL)(O,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_MVDDEF)(OX,MX,FL_CONTEXT(Caller,_,_)).

%% FLNEGTRAILERPREFIX(WRAP_IMETH_EQL)/3
FLNEGTRAILERPREFIX(WRAP_IMETH_EQL)(C,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_IMETH)(CX,MX,FL_CONTEXT(Caller,_,_)).


%% FLNEGTRAILERPREFIX(WRAP_METH_EQL)/3
FLNEGTRAILERPREFIX(WRAP_METH_EQL)(O,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_METH)(OX,MX,FL_CONTEXT(Caller,_,_)).


/*****************************************************************************
  regular wrappers
*****************************************************************************/
FLNEGPREFIX(WRAP_IMVD)(C,M,V,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_IMVD_EQL)(C,M,V,FL_CONTEXT(Caller,Id1,Id2)).

FLNEGPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_IMVDDEF_EQL)(C,M,FL_CONTEXT(Caller,Id1,Id2)).

FLNEGPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,_,_)) :-
	FLNEGPREFIX(WRAP_IMVD)(C,M,_V,FL_CONTEXT(Caller,_,_)).

FLNEGPREFIX(WRAP_MVD)(O,M,V,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_MVD_EQL)(O,M,V,FL_CONTEXT(Caller,Id1,Id2)).

FLNEGPREFIX(WRAP_MVDDEF)(O,M,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_MVDDEF_EQL)(O,M,FL_CONTEXT(Caller,Id1,Id2)).

FLNEGPREFIX(WRAP_MVDDEF)(O,M,FL_CONTEXT(Caller,_,_)) :-
	FLNEGPREFIX(WRAP_MVD)(O,M,_V,FL_CONTEXT(Caller,_,_)).


/*****************************************************************************
  binary attributes (tabled methods)
*****************************************************************************/
FLNEGPREFIX(WRAP_METH)(O,M,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_METH_EQL)(O,M,FL_CONTEXT(Caller,Id1,Id2)).

%% imeth/3
FLNEGPREFIX(WRAP_IMETH)(C,M,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_IMETH_EQL)(C,M,FL_CONTEXT(Caller,Id1,Id2)).


/*****************************************************************************
  monotonic inheritance of signatures
*****************************************************************************/
%% FLNEGPREFIX(WRAP_MVDSIG)/4
FLNEGPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(R,RX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_MVDSIG)(OX,MX,RX,FL_CONTEXT(Caller,_,_)).

%% FLNEGPREFIX(WRAP_MVDSIGDEF)/3
FLNEGPREFIX(WRAP_MVDSIGDEF)(O,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_MVDSIGDEF)(OX,MX,FL_CONTEXT(Caller,_,_)).

FLNEGPREFIX(WRAP_MVDSIGDEF)(O,M,FL_CONTEXT(Caller,_,_)) :-
	FLNEGPREFIX(WRAP_MVDSIG)(O,M,_V,FL_CONTEXT(Caller,_,_)).


%% FLNEGPREFIX(WRAP_IMVDSIG)/4
FLNEGPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(R,RX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_IMVDSIG)(CX,MX,RX,FL_CONTEXT(Caller,_,_)).


%% FLNEGPREFIX(WRAP_IMVDSIGDEF)/3
FLNEGPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_IMVDSIGDEF)(CX,MX,FL_CONTEXT(Caller,_,_)).

FLNEGPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_)) :-
	FLNEGPREFIX(WRAP_IMVDSIG)(C,M,_V,FL_CONTEXT(Caller,_,_)).


%% FLNEGPREFIX(WRAP_BOOLSIG)/3
FLNEGPREFIX(WRAP_BOOLSIG)(O,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_BOOLSIG)(OX,MX,FL_CONTEXT(Caller,_,_)).

%% FLNEGPREFIX(WRAP_IBOOLSIG)/3
FLNEGPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL_AUX(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_IBOOLSIG)(CX,MX,FL_CONTEXT(Caller,_,_)).


/*****************************************************************************
  object existence
*****************************************************************************/
FLNEGPREFIX(WRAP_EXISTS)(X,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL_AUX(X,OX,FL_CONTEXT(Caller,_,_)),
	FLNEGTRAILERPREFIX(WRAP_EXISTS)(OX,FL_CONTEXT(Caller,_,_)).

