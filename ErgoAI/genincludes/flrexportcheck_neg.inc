/***************************************************************************
  file: genincludes/flrexportcheck_neg.inc - A template for generating include
  					     file for checking whether the call
					     to a particular explicitly negated
  					     predicate is authorized or not.
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

***************************************************************************/


FLNEGBodyPREFIX(WRAP_ISA)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME', FLNEGPREFIX(WRAP_ISA)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_SUB)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_SUB)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_METH)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_METH)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_IMETH)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_IMETH)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_BOOLSIG)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_BOOLSIG)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_IBOOLSIG)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_IBOOLSIG)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_MVD)(X,Y,Z,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
	flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_MVD)(X,Y,Z,FWContext)).

FLNEGBodyPREFIX(WRAP_IMVD)(X,Y,Z,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_IMVD)(X,Y,Z,FWContext)).

FLNEGBodyPREFIX(WRAP_MVDDEF)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_MVDDEF)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_IMVDDEF)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_IMVDDEF)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_MVDSIG)(X,Y,Z,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_MVDSIG)(X,Y,Z,FWContext)).

FLNEGBodyPREFIX(WRAP_MVDSIGDEF)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_MVDSIGDEF)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_IMVDSIG)(X,Y,Z,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_IMVDSIG)(X,Y,Z,FWContext)).

FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(X,Y,FWContext) :- 
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_IMVDSIGDEF)(X,Y,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,X19,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,X19,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,FWContext)).

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,FWContext) :-
	FWContext = FL_CONTEXT(Caller,_,_),
        flora_check_export_registry(Caller, 'FLORA_THIS_MODULE_NAME',  FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10, X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,FWContext)).
