/* File:      flrlibman.H
**
** Author(s): Guizhen Yang
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


#include "flora_terms.flh"

:- import flora_configuration/2 from flrregistry.

:- import flora_syslib_loaded/1 from usermod.
:- dynamic flora_syslib_loaded(_).
:- index flora_syslib_loaded/1-1.

%%:- import flora_sysmod_loaded/1 from usermod.
%%:- index flora_sysmod_loaded/1-1.
:- dynamic flora_sysmod_loaded(_).

:- import
	flora_slash/1,
	flora_abort/0,
	flora_concat_atoms/2
   from flrporting.

:- index flora_sysmod_file/3-1.
:- index flora_library_file/3-1.

:- import
	flora_error_line/2
   from flrprint.

:- import flora_load_system_module_util/1 from flrutils.

:- export
	flora_load_library/1,
        is_flora_library/1,
	flora_include/2,
	flora_sysmod_file/3,
	flora_system_module/1,
        flora_nonnegatable_system_module/1,
	flora_load_system_module/1.
