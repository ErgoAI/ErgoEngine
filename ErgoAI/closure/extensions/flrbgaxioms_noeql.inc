/*******************************************************************************
  file: closure/extensions/flrbgaxioms_noeql.inc -
  				      Background axioms for the no-equality case
				      to be used in explanations
  Author(s): Michael Kifer

   Contact:   see  ../CONTACTS.txt
  
   Copyright (C) Coherent Knowledge Systems, 2014-2023.
  
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


%% Sources: ../flrnoeqltrailer.fli
%%          ../flrnoeqltrailer_neg.inc
%%
%%

%% Transitivity of ::, :
%% ?X::?Y :- ?X::?Z, ?Z::?Y
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(X,Z,FL_CONTEXT(Caller,_,_)),
    	FLBodyPREFIX(WRAP_SUB)(Z,Y,FL_CONTEXT(Caller,_,_))),
    'transitivity of subclass relationship'
    ).
%% ?X:?Y :- ?X:?Z, ?Z::?Y.
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_ISA)(X,Y,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(X,Z,FL_CONTEXT(Caller,_,_)),
    	FLBodyPREFIX(WRAP_SUB)(Z,Y,FL_CONTEXT(Caller,_,_))),
    'transitivity of class member'
    ).

%% monotonic inheritance of signatures
%% ?O[?M => ?R] :- ?O:?C, ?C[|?M => ?R|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
    	FLBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_))),
    'inheritance of type to class member'
    ).
%% ?O[?M {?L..?H} => ()] :- ?O:?C, ?C[|?M {?L..?H} => ()|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDCONDEF)(O,M,L,H,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDCONDEF)(C,M,L,H,FL_CONTEXT(Caller,_,_))),
    'inheritance of cardinality to class member'
    ).
%% ?O[?M => ()] :- ?O:?C, ?C[|?M => ()|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDSIGDEF)(O,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of type existence to class member'
    ).

%% ?C[|?M => ?R|] :- ?C::?S, ?S[|?M => ?R|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDSIG)(S,M,R,FL_CONTEXT(Caller,_,_))),
    'inheritance of type to subclass'
    ).
%% ?C[|?M {?L..?H} => ()|] :- ?C::?S, ?S[|?M {?L..?H} => ()|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDCONDEF)(C,M,L,H,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDCONDEF)(S,M,L,H,FL_CONTEXT(Caller,_,_))),
    'inheritance of cardinality to subclass'
    ).
%% ?C[|?M => ()|] :- ?C::?S, ?S[|?M => ()|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDSIGDEF)(S,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of type existence to class member'
    ).

%% inheritance of boolean and transactional
%% O[M] :- O:C, C[|M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_BOOLSIG)(O,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of Boolean method type to class member'
    ).
%% C[|M|] :- C::S, S[|M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IBOOLSIG)(S,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of Boolean method type to subclass'
    ).

%% O[%M] :- O:C, C[|%M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_TRANSIG)(O,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_ITRANSIG)(C,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of transactional method type to class member'
    ).
%% C[|%M|] :- C::S, S[|%M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_ITRANSIG)(C,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_ITRANSIG)(S,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of transactional method type to subclass'
    ).


%%%%% Negated axioms
%% source:  flrnoeqltrailer_neg.inc
%%

%% \neg O:C, S::C |= \neg O:S
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_ISA)(O,S,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(S,C,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_))),
	'\\neg O:C and S::C implies \\neg O:S'
    ).

%% X::Z, \neg X::Y |= \neg Z::Y
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_SUB)(Z,Y,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(X,Z,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,_,_))),
	'X::Z and \\neg X::Y implies \\neg Z::Y'
    ).

%% \neg X::Z, Y::Z |= \neg X::Y
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(Y,Z,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_SUB)(X,Z,FL_CONTEXT(Caller,_,_))),
	'\\neg X::Z and Y::Z implies \\neg X::Y'
    ).

%% X:Z, \neg X:Y |= \neg Z::Y
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_SUB)(Z,Y,FL_CONTEXT(Caller,_,_)),
        (FLBodyPREFIX(WRAP_ISA)(X,Z,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_ISA)(X,Y,FL_CONTEXT(Caller,_,_))),
	'X:Z and \\neg X:Y implies \\neg Z::Y'
    ).

/*****************************************************************************
  closure for \neg signatures
*****************************************************************************/

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of type to class member'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_MVDSIGDEF)(O,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of type to class member'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIG)(S,M,R,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of type to subclass'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(S,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
		FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_))),
        'negative inheritance of type to subclass'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_BOOLSIG)(O,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of Boolean type to class member'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IBOOLSIG)(S,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of Boolean type to subclass'
    ).


