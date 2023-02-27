/* File:      flrprolog.H
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


%% NOTE: flora_dynprlg exists only because of the way import
%% is handled in foo@\prolog(mod). In this case, FLORA inserts
%% a dynamic declaration that foo is a prolog predicate.
:- dynamic flora_dynprlg(_,_).
:- dynamic flora_dynprlg(_,_,_).
:- dynamic modular_dynprlg(_,_).
:- dynamic modular_dyntabled(_,_).
:- dynamic flora_permanent_dynprlg(_,_).


/*
:- dynamic flora_shprlg(_,_).
:- index 
	flora_dynprlg/2-1,
	%%flora_shprlg/2-1,
	flora_prlg/2-1.
*/

:- import
	%%flora_compiler_permaoption/1,
	compiling_flrshell_command/0,
	flora_compilation_context/1
   from flrcompiler.

:- import
        flora_modular_tabled_registry/3,
        flora_modular_prolog_registry/3
   from flrregistry.

:- export
	flora_reset_prolog/0,
	flora_reset_modular_prolog/0,
	flora_define_prolog/1,
	flora_define_prolog/2,
	flora_define_prolog/3,
	flora_prlgdef_repl/4,
	flora_modular_prlgdef/2,
	flora_modular_prlgdef/3,
	flora_modular_prlgdefs/2,
	flora_builtin_modular_prlgdefs/2,
	flora_user_modular_prlgdefs/2,
	flora_user_modular_tableddefs/2,
	flora_negatable_prlgdef/3,
	flora_define_modular_prolog/1,
	flora_define_modular_prolog/2,
	flora_define_modular_prolog/3,
	flora_define_modular_tabled/2,
	flora_define_modular_tabled/3,
	flora_define_permanent_prolog/2,
	is_op_builtin/2,
	flora_prlgdef/3,
	flora_prlgdef/2.

