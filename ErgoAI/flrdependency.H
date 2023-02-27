/* File:      flrdependency.H  -- the dependency checker
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

:- import member/2,length/2 from basics.

:- import subsumes/2 from subsumes.

:- import flora_abolish_table_predicate/1 from flrtables.

:- import
	%%flora_decode_goal_as_atom/2
	flora_decode_goal_as_mainless_atom_substitute_vars/2
   from flrdecode.

:- import flora_get_maxwarnings/1 from flrutils.

:- import
	flora_get_counter/2,
	flora_set_counter/2
   from flrporting.

:- import
	flora_nth_token/2,
	flora_token_rulenum/2
   from flrcomposer.

:- import flora_token_text/6 from flrlexer.

:- import
	is_prrule/3,
        is_prdirective/2,
        is_prignoredep/2,
	%%is_prlabeled_construct/3,
	flora_build_struct/7
   from flrcoder.

:- import get_canonical_form/2 from flrcanon.

:- import
        enable_all_tripwires/0,
        disable_all_tripwires/0
   from flrerrhandler.

:- dynamic rule(_,_,_).
:- dynamic target(_,_,_).
:- dynamic tabled_head(_).
:- dynamic dependency(_,_,_,_).
:- dynamic ignored(_,_,_).
:- dynamic warning_seen(_,_,_,_,_).

:- index(rule/3,trie).
:- index(dependency/4,trie).

:- export
	is_control/3,
        depend/3,
	check_for_dependencies/2.
