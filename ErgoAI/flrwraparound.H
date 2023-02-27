/* File:      flrwraparound.P -- Workspace wrapper for .fli files
**
** Author(s): Michael Kifer
**
** Contact:   see  CONTACTS.txt
**
** Copyright (C) by
**      The Research Foundation of the State University of New York, 1999-2018.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
**
*/


:- compiler_options([xpp_on]).
#mode standard Prolog


#include "flora_porting.flh"
#include "flora_terms.flh"

:- import
	parse_filename/4
   from machine.

%% The import below is not needed and also sometimes causes compile errors
%% that prevent flrwraparound.P from getting compiled.
%%:- import ','/2 from standard.

:- import
	flora_concat_items/2,
	flora_concat_atoms/2
   from flrporting.

:- import
	flora_set_xpp_options_for_read/0,
	flora_clear_xpp_options/0,
	flora_check_filename/2,
	flora_locate_file/3
   from flrutils.

:- import
	flora_stderr_nl/0,
	flora_error_line/2
   from flrprint.

:- import process_control/2 from shell.

:- import xpp_process_file/3 from parse.

:- import bootstrap_flora/0 from flora2.

:- export flWrapAround/1.

