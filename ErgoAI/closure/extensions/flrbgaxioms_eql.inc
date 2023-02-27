/*******************************************************************************
  file: closure/extensions/flrbgaxioms_eql.inc - Axioms for equality
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

%% source:  ../flreqltrailer.fli
%%          ../flreqltrailer_neg.inc

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(X,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(Y,OY,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_SUB)(OX,OY,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_ISA)(OX,CX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVD)(C,M,V,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(V,VX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVD)(CX,MX,VX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDDEF)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVD)(O,M,V,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(V,VX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_MVD)(OX,MX,VX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLBodyPREFIX(WRAP_MVDDEF)/3
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDDEF)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_MVDDEF)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLBodyPREFIX(WRAP_IMETH)/3
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMETH)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMETH)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLBodyPREFIX(WRAP_METH)/3
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_METH)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_METH)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_TRAN)(O,T,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(T,TX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_TRAN)(OX,TX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(R,RX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_MVDSIG)(OX,MX,RX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDCONDEF)(O,M,L,H,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(H,HX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(L,LX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_MVDCONDEF)(OX,MX,LX,HX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDSIGDEF)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_MVDSIGDEF)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(R,RX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDSIG)(CX,MX,RX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDCONDEF)(O,M,L,H,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(H,HX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(L,LX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDCONDEF)(OX,MX,LX,HX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDSIGDEF)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_BOOLSIG)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_BOOLSIG)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IBOOLSIG)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IBOOLSIG)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_TRANSIG)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_TRANSIG)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_ITRANSIG)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_ITRANSIG)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_EXISTS)(X,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(X,OX,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_EXISTS)(OX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

%%%%%%%%%%%%%%%%%%% NEG %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(X,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(Y,OY,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_SUB)(OX,OY,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_ISA)(OX,CX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_IMVD)(C,M,V,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(V,VX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_IMVD)(CX,MX,VX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLNEGBodyPREFIX(WRAP_IMVDDEF)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_IMVDDEF)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

%% FLNEGBodyPREFIX(WRAP_MVD)/4
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_MVD)(O,M,V,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(V,VX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_MVD)(OX,MX,VX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLNEGBodyPREFIX(WRAP_MVDDEF)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_MVDDEF)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_MVDDEF)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

%% FLNEGBodyPREFIX(WRAP_IMETH)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_IMETH)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_IMETH)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLNEGBodyPREFIX(WRAP_METH)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_METH)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_METH)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

%% FLNEGBodyPREFIX(WRAP_MVDSIG)/4
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_MVDSIG)(O,M,R,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(R,RX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_MVDSIG)(OX,MX,RX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLNEGBodyPREFIX(WRAP_MVDSIGDEF)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_MVDSIGDEF)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_MVDSIGDEF)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLNEGBodyPREFIX(WRAP_IMVDSIG)/4
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_IMVDSIG)(C,M,R,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(R,RX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_IMVDSIG)(CX,MX,RX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

%% FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLNEGBodyPREFIX(WRAP_BOOLSIG)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_BOOLSIG)(O,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(O,OX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_BOOLSIG)(OX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).


%% FLNEGBodyPREFIX(WRAP_IBOOLSIG)/3
FL_BGAXIOM_MODULARIZED(
    FLNEGBodyPREFIX(WRAP_IBOOLSIG)(C,M,FL_CONTEXT(Caller,_,_)),
    (WRAP_OBJEQL(C,CX,FL_CONTEXT(Caller,_,_)),
	WRAP_OBJEQL(M,MX,FL_CONTEXT(Caller,_,_)),
	FLNEGBodyPREFIX(WRAP_IBOOLSIG)(CX,MX,FL_CONTEXT(Caller,_,_))),
    'equality axiom'
).

