/*******************************************************************************
  file: closure/extensions/flrbgaxioms_mono_inh.inc -
  				      background axioms to be used
                                      in explaning monotonic inheritance
  Author(s): Michael Kifer

  Contact:   michael.kifer@coherentknowledge.com

  Copyright (C) Coherent Knowledge Systems, LLC, 2015.
  All rights reserved.

*******************************************************************************/

%% source:  ../flrtrailer_mono_inh.fli

:- import
        sub_compound/4,
        sub_compound_eq/5
   from flrauxil.

%%% Monotonic inheritance of malues
%% source:  ../flrtrailer_mono_inh.fli

%% monotonic inheritance of data
%% ?O[?M -> ?R] :- ?O:?C, ?C[|?M -> ?R|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVD)(O,M,R,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
    	FLBodyPREFIX(WRAP_IMVD)(C,M,R,FL_CONTEXT(Caller,_,_))),
    'inheritance of data to class member'
    ).

%% ?O[?M -> {}] :- ?O:?C, ?C[|?M -> {}|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_MVDDEF)(O,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of data existence to class member'
    ).

%% ?C[|?M -> ?R|] :- ?C::?S, ?S[|?M -> ?R|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVD)(C,M,R,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVD)(S,M,R,FL_CONTEXT(Caller,_,_))),
    'inheritance of data to subclass'
    ).
%% ?C[|?M -> {}|] :- ?C::?S, ?S[|?M -> {}|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMVDDEF)(S,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of data existence to class member'
    ).

%% inheritance of boolean and transactional
%% O[M] :- O:C, C[|M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_METH)(O,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMETH)(C,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of Boolean method to class member'
    ).
%% C[|M|] :- C::S, S[|M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_IMETH)(C,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_IMETH)(S,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of Boolean method to subclass'
    ).

/*
%% Note implemented: WRAP_ITRAN does not exist
%% O[%M] :- O:C, C[|%M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_TRAN)(O,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_ITRAN)(C,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of transactional method to class member'
    ).
%% C[|%M|] :- C::S, S[|%M|].
FL_BGAXIOM_MODULARIZED(
    FLBodyPREFIX(WRAP_ITRAN)(C,M,FL_CONTEXT(Caller,_,_)),
    (FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	FLBodyPREFIX(WRAP_ITRAN)(S,M,FL_CONTEXT(Caller,_,_))),
    'inheritance of transactional method to subclass'
    ).
*/


/*****************************************************************************
  closure for \neg signatures
*****************************************************************************/

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVD)(C,M,R,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_MVD)(O,M,R,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of data to class member'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_MVDDEF)(O,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of data to class member'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVD)(S,M,R,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_IMVD)(C,M,R,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of data to subclass'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMVDDEF)(S,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
		FLNEGBodyPREFIX(WRAP_IMVDDEF)(C,M,FL_CONTEXT(Caller,_,_))),
        'negative inheritance of data to subclass'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMETH)(C,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_ISA)(O,C,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_METH)(O,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of Boolean method to class member'
    ).

FL_BGAXIOM_MODULARIZED(
	FLNEGBodyPREFIX(WRAP_IMETH)(S,M,FL_CONTEXT(Caller,_,_)),
	(FLBodyPREFIX(WRAP_SUB)(C,S,FL_CONTEXT(Caller,_,_)),
	    FLNEGBodyPREFIX(WRAP_IMETH)(C,M,FL_CONTEXT(Caller,_,_))),
	'negative inheritance of Boolean method to subclass'
    ).


