/*******************************************************************************
  file: closure/flrnoeqltrailer_neg_inh.inc - A template for generating trailers
                                              for explicit negation under
                                              inheritance. This mode
					      does not support equality.
  Author(s): Michael Kifer
  Contact:   see  ../CONTACTS.txt

  Copyright (C) by
       The Research Foundation of the State University of New York, 2013-2018;
       Coherent Knowledge Systems, LLC, 2014-2018.

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

%%%% THIS FILE CONTAINS NEGATIVE AXIOMS THAT ARE GOOD ONLY FOR THE INHERITANCE 
%%%% VERSION

/*****************************************************************************
  inheritable methods that are locally blocked by negation

  The argument "MethSign" has the form of Funct/Arity/Type.
*****************************************************************************/

/*
%% Block inheritance of [| -> |] to subclasses, if neg Method is defined
%% FLLOCALPREFIX(WRAP_IMVD)/3
FLLOCALPREFIX(WRAP_IMVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	var(MethSign),
	FLNEGTRAILERPREFIX(WRAP_IMVD)(S,M,_V,FL_CONTEXT(Caller,Id1,Id2)),
	%% Should have a cut here when XSB will allow
	flora_funct_arity(M,MethSign).
*/

%% S[|M->{}|]
FLLOCALPREFIX(WRAP_IMVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	var(MethSign),
	!,
	FLNEGTRAILERPREFIX(WRAP_IMVDDEF)(S,M,FL_CONTEXT(Caller,Id1,Id2)),
	flora_funct_arity(M,MethSign).

/*
FLLOCALPREFIX(WRAP_IMVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	nonvar(MethSign), % the cut above might not work under local scheduling
	flora_funct_arity(M,MethSign),
	FLNEGTRAILERPREFIX(WRAP_IMVD)(S,M,_V,FL_CONTEXT(Caller,Id1,Id2)).
	%% Should have a cut here when XSB will allow
*/

FLLOCALPREFIX(WRAP_IMVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	nonvar(MethSign), % the cut above might not work under local scheduling
	flora_funct_arity(M,MethSign),
	FLNEGTRAILERPREFIX(WRAP_IMVDDEF)(S,M,FL_CONTEXT(Caller,Id1,Id2)).

local_NEG_block_imvd(S,M,V,FL_CONTEXT(Caller,Id1,Id2)) :-
        FLNEGTRAILERPREFIX(WRAP_IMVD)(S,M,V,FL_CONTEXT(Caller,Id1,Id2)).
intermed_NEG_block_imvd(C,M,V,Class,FL_CONTEXT(Caller,Id1,Id2)) :-
	WRAP_SUB(X,C,FL_CONTEXT(Caller,Id1,Id2)),
	WRAP_SUB(Class,X,FL_CONTEXT(Caller,Id1,Id2)),
	local_NEG_block_imvd(X,M,V,FL_CONTEXT(Caller,Id1,Id2)).

%% FLLOCALPREFIX(WRAP_MVD)/3
/*
%% Block inheritance of [| -> |] to class members, if neg Method is defined
FLLOCALPREFIX(WRAP_MVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	var(MethSign),
	FLNEGTRAILERPREFIX(WRAP_MVD)(S,M,_V,FL_CONTEXT(Caller,Id1,Id2)),
	%% Should have a cut here when XSB will allow
	flora_funct_arity(M,MethSign).
*/

%% S[M->{}]
FLLOCALPREFIX(WRAP_MVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	var(MethSign),
	!,
	FLNEGTRAILERPREFIX(WRAP_MVDDEF)(S,M,FL_CONTEXT(Caller,Id1,Id2)),
	flora_funct_arity(M,MethSign).

/*
FLLOCALPREFIX(WRAP_MVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	nonvar(MethSign), % the cut above might not work under local scheduling
	flora_funct_arity(M,MethSign),
	FLNEGTRAILERPREFIX(WRAP_MVD)(S,M,_V,FL_CONTEXT(Caller,Id1,Id2)).
	%% Should have a cut here when XSB will allow
*/

FLLOCALPREFIX(WRAP_MVD)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	nonvar(MethSign), % the cut above might not work under local scheduling
	flora_funct_arity(M,MethSign),
	FLNEGTRAILERPREFIX(WRAP_MVDDEF)(S,M,FL_CONTEXT(Caller,Id1,Id2)).

local_NEG_block_mvd(S,M,V,FL_CONTEXT(Caller,Id1,Id2)) :-
        FLNEGTRAILERPREFIX(WRAP_MVD)(S,M,V,FL_CONTEXT(Caller,Id1,Id2)).
intermed_NEG_block_mvd(C,M,V,Object,FL_CONTEXT(Caller,Id1,Id2)) :-
	WRAP_SUB(X,C,FL_CONTEXT(Caller,Id1,Id2)),
	WRAP_ISA(Object,X,FL_CONTEXT(Caller,Id1,Id2)),
	local_NEG_block_imvd(X,M,V,FL_CONTEXT(Caller,Id1,Id2)).

%% Block inheritance of [|boolean|] to classes, if neg boolean is defined
%% FLLOCALPREFIX(WRAP_IMETH)/3
FLLOCALPREFIX(WRAP_IMETH)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	var(MethSign),
	!,
	FLNEGTRAILERPREFIX(WRAP_IMETH)(S,M,FL_CONTEXT(Caller,Id1,Id2)),
	%% Should have a cut here when XSB will allow
	flora_funct_arity(M,MethSign).

FLLOCALPREFIX(WRAP_IMETH)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	nonvar(MethSign), % the cut above might not work under local scheduling
	flora_funct_arity(M,MethSign),
	FLNEGTRAILERPREFIX(WRAP_IMETH)(S,M,FL_CONTEXT(Caller,Id1,Id2)).
	%% Should have a cut here when XSB will allow


%% FLLOCALPREFIX(WRAP_METH)/3
%% Block inheritance of [|boolean|] to class members, if neg boolean is defined
%% FLLOCALPREFIX(WRAP_METH)/3
FLLOCALPREFIX(WRAP_METH)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	var(MethSign),
	!,
	FLNEGTRAILERPREFIX(WRAP_METH)(S,M,FL_CONTEXT(Caller,Id1,Id2)),
	%% Should have a cut here when XSB will allow
	flora_funct_arity(M,MethSign).

FLLOCALPREFIX(WRAP_METH)(S,MethSign,FL_CONTEXT(Caller,Id1,Id2)) :-
	nonvar(MethSign), % the cut above might not work under local scheduling
	flora_funct_arity(M,MethSign),
	FLNEGTRAILERPREFIX(WRAP_METH)(S,M,FL_CONTEXT(Caller,Id1,Id2)).
	%% Should have a cut here when XSB will allow

