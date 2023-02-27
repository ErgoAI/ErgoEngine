/* File:      flrundefined.H
**
** Author(s): Chang Zhao
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

:- dynamic term_seen(_).
%% trie indexing this speeds up things by a LOT!
:- index(term_seen/1,trie).

:- import
	%%reverse/2,
	member/2,
	length/2
   from basics.

:- import
	flora_db_find_base/2,
	flora_db_insert_base_fast/2
   from flrstoragebase.

:- import
	flora_concat_atoms/2,
        flora_abort/2,
        flora_get_counter/2,
	flora_concat_items/2
   from flrporting.

:- import 
        flora_builtin_class/1
   from flrdatatype.
:- import 
        flora_coder_error/1
   from flrutils.

:- import flora_disabled_undefinedness_check_registry/3 from flrregistry.

:- import flora_prlgdef/2 from flrprolog.

:- import
	is_flogic/4,
	is_prrule/3,
	is_prfact/2,
	is_pratom/3,
	is_prvariable/3,
	is_prbuiltin_identifier/3,
	is_prtransactionalvariable/3,
	is_prterm/4,
	is_pratomlit/5,
	is_prtransactionalatomlit/5,
        is_prtermlit/5,
        is_prtransactionaltermlit/5,
        flora_write_struct/3
   from flrcoder.

:- import
        flora_current_out_line/2,
        flora_current_out_line/1
   from flrprint.

:- import
        flora_reset_newvar/0,
        flora_new_varobj/3
   from flrcompiler.

:- import 
        %%is_flora_callable_literal/1,
        flora_decode_predicate/6,
        flora_module_predicate/4,
	flora_fdb_storage_name/2,
	flora_fld_storage_name/2
   from flrwrapper.

:- import flora_decode_goal_as_atom/2 from flrdecode.

:- import 
        flora_generalize_context/2,
	%%flora_generalize_last/2,
        flora_add_last/3
   from flrtrim.

:- import flrtrim_last/2 from flora_ground.

:- import term_to_atom/2 from string.

:- export
	flora_debug_code/3,
	flora_temp_protect_call/2,
	flora_temp_unprotect_call/1,
	flora_define_predicate/1,
	flora_error_undefined/1,
	flora_error_notexported/1,
	flora_error_invalid_export/1.
