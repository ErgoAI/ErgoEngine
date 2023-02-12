/*******************************************************************************
  file: genincludes/flrpreddef_neg.inc - A template for generating the include
  				       	 file for checking undefinedness.
  					 It is included by flrutils.P at the
					 END of every compiled FLORA-2 file
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

FLNEGBodyPREFIX(WRAP_ISA)(X,Y,_FWContext) :- 
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_ISA)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_SUB)(X,Y,_FWContext),
	flora_error_undefined(WRAP_ISA(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_ISA)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :- 
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_ISA)(_,Y,_)),
	FLNEGPREFIX(WRAP_ISA)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	    
FLNEGBodyPREFIX(WRAP_SUB)(X,Y,_FWContext) :- 
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_SUB)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_SUB)(X,Y,_FWContext),
	flora_error_undefined(WRAP_SUB(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :- 
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_SUB)(_,Y,_)),
	FLNEGPREFIX(WRAP_SUB)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

FLNEGBodyPREFIX(WRAP_METH)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_METH)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_METH)(X,Y,_FWContext),
	FLUndefinedPREFIX(WRAP_IMETH)(X,Y,_FWContext),
	flora_error_undefined(WRAP_METH(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_METH)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_METH)(_,Y,_)),
	FLNEGPREFIX(WRAP_METH)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

FLNEGBodyPREFIX(WRAP_IMETH)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_IMETH)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_IMETH)(X,Y,_FWContext),
	flora_error_undefined(WRAP_IMETH(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_IMETH)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_METH)(_,Y,_)),
	FLNEGPREFIX(WRAP_IMETH)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

FLNEGBodyPREFIX(WRAP_BOOLSIG)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_BOOLSIG)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_BOOLSIG)(X,Y,_FWContext),
	FLUndefinedPREFIX(WRAP_IBOOLSIG)(X,Y,_FWContext),
	flora_error_undefined(WRAP_BOOLSIG(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_BOOLSIG)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_METH)(_,Y,_)),
	FLNEGPREFIX(WRAP_BOOLSIG)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

FLNEGBodyPREFIX(WRAP_IBOOLSIG)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_IBOOLSIG)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_IBOOLSIG)(X,Y,_FWContext),
	flora_error_undefined(WRAP_IBOOLSIG(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_IBOOLSIG)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_METH)(_,Y,_)),
	FLNEGPREFIX(WRAP_IBOOLSIG)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

FLNEGBodyPREFIX(WRAP_MVD)(X,Y,Z,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_MVD)(X,Y,Z,_FWContext)),
	FLUndefinedPREFIX(WRAP_MVD)(X,Y,Z,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVD)(X,Y,Z,_FWContext),
	FLUndefinedPREFIX(WRAP_MVDDEF)(X,Y,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVDDEF)(X,Y,_FWContext),
	flora_error_undefined(WRAP_MVD(X,Y,Z,_FWContext)).

FLNEGBodyPREFIX(WRAP_MVD)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_MVD)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	    
FLNEGBodyPREFIX(WRAP_IMVD)(X,Y,Z,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_IMVD)(X,Y,Z,_FWContext)),
	FLUndefinedPREFIX(WRAP_IMVD)(X,Y,Z,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVDDEF)(X,Y,_FWContext),
	flora_error_undefined(WRAP_IMVD(X,Y,Z,_FWContext)).


FLNEGBodyPREFIX(WRAP_IMVD)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_IMVD)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	    
FLNEGBodyPREFIX(WRAP_MVDDEF)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_MVDDEF)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_MVDDEF)(X,Y,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVDDEF)(X,Y,_FWContext),
	FLUndefinedPREFIX(WRAP_MVD)(X,Y,Z,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVD)(X,Y,Z,_FWContext),
	flora_error_undefined(WRAP_MVDDEF(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_MVDDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_MVDDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	    
FLNEGBodyPREFIX(WRAP_IMVDDEF)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_IMVDDEF)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_IMVDDEF)(X,Y,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVD)(X,Y,Z,_FWContext),
	flora_error_undefined(WRAP_IMVDDEF(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_IMVDDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_IMVDDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	    
FLNEGBodyPREFIX(WRAP_MVDSIG)(X,Y,Z,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_MVDSIG)(X,Y,Z,_FWContext)),
	FLUndefinedPREFIX(WRAP_MVDSIG)(X,Y,Z,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVDSIG)(X,Y,Z,_FWContext),
	flora_error_undefined(WRAP_MVDSIG(X,Y,Z,_FWContext)).

FLNEGBodyPREFIX(WRAP_MVDSIG)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_MVDSIG)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

FLNEGBodyPREFIX(WRAP_MVDSIGDEF)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_MVDSIGDEF)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_MVDSIGDEF)(X,Y,_FWContext),
	FLUndefinedPREFIX(WRAP_IMVDSIGDEF)(X,Y,_FWContext),
	flora_error_undefined(WRAP_MVDSIGDEF(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_MVDSIGDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_MVDSIGDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	    
FLNEGBodyPREFIX(WRAP_IMVDSIG)(X,Y,Z,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_IMVDSIG)(X,Y,Z,_FWContext)),
	FLUndefinedPREFIX(WRAP_IMVDSIG)(X,Y,Z,_FWContext),
	flora_error_undefined(WRAP_IMVDSIG(X,Y,Z,_FWContext)).

FLNEGBodyPREFIX(WRAP_IMVDSIG)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_IMVDSIG)(X,Y,Z,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(X,Y,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_IMVDSIGDEF)(X,Y,_FWContext)),
	FLUndefinedPREFIX(WRAP_IMVDSIGDEF)(X,Y,_FWContext),
	flora_error_undefined(WRAP_IMVDSIGDEF(X,Y,_FWContext)).

FLNEGBodyPREFIX(WRAP_IMVDSIGDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_MVD)(_,Y,_,_)),
	FLNEGPREFIX(WRAP_IMVDSIGDEF)(X,Y,FL_CONTEXT(_CallerMod,_Id1,_Id2)).

%% For exists (a[]) it doesn't make sense to check for definedness
FLNEGBodyPREFIX(WRAP_EXISTS)(X,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	FLNEGPREFIX(WRAP_EXISTS)(X,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	    


/*
    Undefinedness checking for tabled HiLog
*/

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,_FWContext) :-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,FL_CONTEXT(_CallerMod,_Id1,_Id2)).
	

FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_FWContext)
	:-
	debugging,
        \+ FLORA_IGNORE_UNDEFCHECK(FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_FWContext)),
	FLUndefinedPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_FWContext),
	flora_error_undefined(WRAP_TABLED_HILOG(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_FWContext)).
FLNEGBodyPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,FL_CONTEXT(_CallerMod,_Id1,_Id2)) :-
    	flora_query_permitted_during_pause,
	flora_bind_to_export_registry(_CallerMod,'FLORA_THIS_MODULE_NAME',FLBodyPREFIX(WRAP_TABLED_HILOG)(X01,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)),
	FLNEGPREFIX(WRAP_TABLED_HILOG)(X01,X02,X03,X04,X05,X06,X07,X08,X09,X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,FL_CONTEXT(_CallerMod,_Id1,_Id2)).


