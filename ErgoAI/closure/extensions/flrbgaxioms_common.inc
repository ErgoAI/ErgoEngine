/*******************************************************************************
  file: closure/extensions/flrbgaxioms_common.inc -
  				      Common background axioms
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

%% source:  ../flrcommon.fli
%%          ../flrequality_none.inc
%%          ../flrsigaxioms.fli

:- import
        sub_compound/4,
        sub_compound_eq/5
   from flrauxil.

%% +>>
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDINC)(O,M,[H|T],FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_MVD)(O,M,H,FL_CONTEXT(Caller,_,_)),
     FLBodyPREFIX(WRAP_MVDINC)(O,M,T,FL_CONTEXT(Caller,_,_))),
     'definition of +>>'
).
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDINC)(O,M,[H|T],FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_IMVD)(O,M,H,FL_CONTEXT(Caller,_,_)),
     FLBodyPREFIX(WRAP_IMVDINC)(O,M,T,FL_CONTEXT(Caller,_,_))),
     'definition of +>>'
).

%% ->->
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDTOLIST)(O,M,L,FL_CONTEXT(Caller,_,_)),
    FLLIBBAGOF(V,[],FLBodyPREFIX(WRAP_MVD)(O,M,V,FL_CONTEXT(Caller,_,_)),L),
    'definition of ->->'
).
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDTOLIST)(O,M,L,FL_CONTEXT(Caller,_,_)),
    FLLIBBAGOF(V,[],FLBodyPREFIX(WRAP_IMVD)(O,M,V,FL_CONTEXT(Caller,_,_)),L),
    'definition of ->->'
).


%% FL_TRUTHVALUE_TABLED_CALL(X) :- call(X): needed?
%% WRAP_EXISTS(X,FWContext): needed?

%% source:  ../flrequality_none.inc

%% X:=:Y :- X:=:Z, X:=:Y. (actually, :=: but not =)
FL_BGAXIOM_MODULARIZED(
    WRAP_OBJEQL(X,Y,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(X,Z,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(Z,Y,FL_CONTEXT(Caller,_,_))),
    'transitivity of equality'
    ).
%% \neg X:=:Y :- X:=:V, Y:=:W, \neg V:=W.
FL_BGAXIOM_MODULARIZED(
    FLNEGPREFIX(WRAP_OBJEQL)(X,Y,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(X,V,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(Y,W,FL_CONTEXT(Caller,_,_)),
	FLNEGPREFIX(WRAP_OBJEQL)(V,W,FL_CONTEXT(Caller,_,_))),
    'contrapositive of equality'
    ).



%%% Inheritance of types
%% source:  ../flrsigaxioms.fli

%% O[M=>R], R::R1 |= O[M=>R1]
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_MVDSIG)(O,M,R1,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
	    FLBodyPREFIX(WRAP_SUB)(R,R1,FL_CONTEXT(Caller,_,_))),
	'inheritance of type to subclass'
    ).

%% O[M(A1,...,An)=>R], B1??A1, ..., BN??AN |= O[M(B1,...,BN)=>R]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_MVDSIG)(O,M1,R,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by method specialization'
    ).

%% C[|M=>R|], R::R1 |= C[|M=>R1|]
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_IMVDSIG)(C,M,R1,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)),
	    FLBodyPREFIX(WRAP_SUB)(R,R1,FL_CONTEXT(Caller,_,_))),
	'inheritance of type to subclass'
    ).

%% O[|M(A1,...,An)=>R|], B1??A1, ..., BN??AN |= O[|M(B1,...,BN)=>R|]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_IMVDSIG)(O,M1,R,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_IMVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by method specialization'
    ).

%% \neg =>
%% \neg O[M=>R1], R::R1 |= \neg O[M=>R]
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_MVDSIG)(O,M,R1,FL_CONTEXT(Caller,_,_)),
	    FLBodyPREFIX(WRAP_SUB)(R,R1,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of type'
    ).

%% \neg O[M(B1,...,Bn)=>R], B1??A1, ..., BN??AN |= \neg O[M(A1,...,AN)=>R]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_MVDSIG)(O,M1,R,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by method specialization'
    ).

%% \neg [|...|]
%% \neg C[|M=>R1|], R::R1 |= \neg C[|M=>R|]
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_IMVDSIG)(C,M,R1,FL_CONTEXT(Caller,_,_)),
	    FLBodyPREFIX(WRAP_SUB)(R,R1,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of type'
    ).


%% \neg O[M1=>R], M1::M |= \neg O[M=>R]
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_IMVDSIG)(O,M1,R,FL_CONTEXT(Caller,_,_)),
	    FLBodyPREFIX(WRAP_SUB)(M1,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of type'
    ).

%% \neg O[M1=>R], M1:M |= \neg O[M=>R]
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_IMVDSIG)(O,M1,R,FL_CONTEXT(Caller,_,_)),
	    FLBodyPREFIX(WRAP_ISA)(M1,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of type'
    ).

%% \neg C[|M1(B1,...,Bn)=>R|], B1??A1, ..., BN??AN |= \neg C[|M(A1,...,AN)=>R|]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_IMVDSIG)(C,M1,R,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by method specialization'
    ).

%% Boolean
%% O[=>M(A1,...,An)], B1??A1, ..., BN??AN |= O[=>M1(B1,...,BN)]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_BOOLSIG)(O,M1,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_BOOLSIG)(O,M,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by Boolean method specialization'
    ).

%% C[|=>M1(A1,...,An)|], B1??A1, ..., BN??AN |= C[|=>M(B1,...,BN)|]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_IBOOLSIG)(C,M1,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by Boolean method specialization'
    ).

%% The \neg axioms are somewhat strong. Maybe too strong
%% \neg O[=>M1(B1,...,Bn)], B1??A1, ..., BN??AN |= \neg O[=>M(A1,...,AN)]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_BOOLSIG)(O,M,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_BOOLSIG)(O,M1,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by Boolean  method specialization'
    ).

%% \neg C[|=>M1(B1,...,Bn)|], B1??A1, ..., BN??AN |= \neg C[|=>M(A1,...,AN)|]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)),
	(FLNEGBodyPREFIX(WRAP_IBOOLSIG)(C,M1,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by Boolean method specialization'
    ).


%% O[=>%M(A1,...,An)], B1??A1, ..., BN??AN |= O[=>%M1(B1,...,BN)]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_TRANSIG)(O,M1,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_TRANSIG)(O,M,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by Boolean method specialization'
    ).

%% C[|=>%M1(A1,...,An)|], B1??A1, ..., BN??AN |= C[|=>%M(B1,...,BN)|]
%% where ?? is :, ::, or =
FL_BGAXIOM_MODULARIZED(
	FLBodyPREFIX(WRAP_ITRANSIG)(C,M1,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_ITRANSIG)(C,M,FL_CONTEXT(Caller,_,_)),
	    FL_METHOD_SPECIALIZATION(M1,M)),
	'inheritance by Boolean method specialization'
    ).


