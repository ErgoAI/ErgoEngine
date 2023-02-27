/*******************************************************************************
  file: closure/flrequality_none.inc - Equality axioms used everywhere

  Author(s): Guizhen Yang, Michael Kifer

  Contact:   see  ../CONTACTS.txt

  Copyright (C) by
       The Research Foundation of the State University of New York, 1999-2018.

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
/*
   These are the equality axioms used everywhere, even without equality
*/


/****************************************************************************
  WRAP_OBJEQL/3
  Primitive (common) equality maintenance
  Here we just treat :=: as an ordinary symmetric, transitive relation.
****************************************************************************/

/* First, make sure that data types are handled quickly and not tabled */

%% Bridge d^objeql/3 --> objeql/3. Needed because WRAP_OBJEQL is not
%% checked for undefinedness in genincludes/flrpreddef.fli
FLBodyPREFIX(WRAP_OBJEQL)(X,Y,_FWContext) :- WRAP_OBJEQL(X,Y,_FWContext).
FLNEGBodyPREFIX(WRAP_OBJEQL)(X,Y,_FWContext) :- FLNEGPREFIX(WRAP_OBJEQL)(X,Y,_FWContext).

WRAP_OBJEQL(X,Y,_FWContext) :-
	%% when experimenting with subsumptive tabling, need to disable
	%% attibuted vars, since their combo has not been implemented yet
	%% Also replace equality with unification to simplify the tests
	flrutils:flora_experimenting_with(no_attr_vars),
	X=Y,
	!.
/*
    Optimization for data types.
    If X or Y is a data types then we just check them for idenity.
    If that fails, we don't go into the expensive DIFFOBJEQL stuff
    but simply fail.
    Change this: simply check for flora_datatype_name(X,Typ), Typ\=FL_SYMBOL
    or the same for Y.
*/
WRAP_OBJEQL(X,Y,_FWContext) :- (number(X); number(Y)), X=Y, !.
WRAP_OBJEQL(X,Y,_FWContext) :-
        (flora_datatype_name(X,FL_INTEGER); flora_datatype_name(Y,FL_INTEGER)),
	X=Y,
        !.
WRAP_OBJEQL(X,Y,_FWContext) :-
        (flora_datatype_name(X,FL_LONG); flora_datatype_name(Y,FL_LONG)),
	X=Y,
        !.
WRAP_OBJEQL(X,Y,_FWContext) :-
        (flora_datatype_name(X,FL_STRING); flora_datatype_name(Y,FL_STRING)),
	X=Y,
        !.
WRAP_OBJEQL(X,Y,_FWContext) :-
        (flora_datatype_name(X,FL_IRI); flora_datatype_name(Y,FL_IRI)),
	!,
	X=Y.
WRAP_OBJEQL(X,Y,_FWContext) :-
        (flora_datatype_name(X,FL_BOOL); flora_datatype_name(Y,FL_BOOL)),
	X=Y,
        !.

%% The rest of the axioms: transitivity, reflexivity, symmetry
WRAP_OBJEQL(X,Y,_FWContext) :- X=Y.
%%WRAP_OBJEQL(X,Y,_FWContext) :- (nonvar(X); nonvar(Y)), X=Y.
WRAP_OBJEQL(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
	WRAP_DIFFOBJEQL(X,Y,FL_CONTEXT(Caller,Id1,Id2)).

%% WRAP_DIFFOBJEQL means X,Y are equal, but not identical
WRAP_DIFFOBJEQL(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLTRAILERPREFIX(WRAP_OBJEQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)),
	X \= Y.

WRAP_DIFFOBJEQL(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLTRAILERPREFIX(WRAP_OBJEQL)(Y,X,FL_CONTEXT(Caller,Id1,Id2)),
	X \= Y.

%% transitivity of equality
WRAP_DIFFOBJEQL(X,Y,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL(X,Z,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL(Z,Y,FL_CONTEXT(Caller,_,_)),
	X \= Y.


/* neg WRAP_OBJEQL  */
FLNEGPREFIX(WRAP_OBJEQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
        FLNEGPREFIX(WRAP_DIFFOBJEQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)).

FLNEGPREFIX(WRAP_DIFFOBJEQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGTRAILERPREFIX(WRAP_OBJEQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)),
	X \= Y.

FLNEGPREFIX(WRAP_DIFFOBJEQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLNEGPREFIX(WRAP_OBJEQL)(Y,X,FL_CONTEXT(Caller,Id1,Id2)),
	X \= Y.

FLNEGPREFIX(WRAP_DIFFOBJEQL)(X,Y,FL_CONTEXT(Caller,Id1,Id2)) :-
	FLTRAILERPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(Caller,Id1,Id2)).

/* neg WRAP_DIFFOBJEQL  */
%% WRAP_DIFFOBJEQL means X,Y are unequal and not identical
FLNEGPREFIX(WRAP_DIFFOBJEQL)(X,Y,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL(X,V,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL(Y,W,FL_CONTEXT(Caller,_,_)),
	X \= Y,
	FLNEGTRAILERPREFIX(WRAP_OBJEQL)(V,W,FL_CONTEXT(Caller,_,_)).

FLNEGPREFIX(WRAP_DIFFOBJEQL)(X,Y,FL_CONTEXT(Caller,_,_)) :-
	WRAP_DIFFOBJEQL(X,V,FL_CONTEXT(Caller,_,_)),
	WRAP_DIFFOBJEQL(Y,W,FL_CONTEXT(Caller,_,_)),
	X \= Y,
	FLNEGTRAILERPREFIX(WRAP_OBJEQL)(W,V,FL_CONTEXT(Caller,_,_)).


/*
    The query inserting the definition of WRAP_DIFFOBJEQL_AUX/3 has been
    moved to genincludes/flrdynamic_connectors.fli.
    This is because genincludes/flrdynamic_connectors.fli is included in
    the header, and we need to execute this query BEFORE any user program
    queries.
*/
