/* File:      flrcomposer.H
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


:- dynamic indexed_token(_,_,_).
:- dynamic composing_error(_,_).
:- dynamic composing_error(_,_,_).

:- index(indexed_token/3,trie).
:- index(composing_error/2,trie).
:- index(composing_error/3,trie).

:- import eval/2 from eval.

:- import
	flora_retractall_substitute/1
   from flrutils.

:- import 
	flora_set_counter/2,
	flora_get_counter/2
   from flrporting.

:- import
	flora_parenthesized_op/1,
	flora_opdef/3
   from flroperator.

:- import
	is_updateop/1,
	is_aggregtop/1
   from flrparser.

:- import
	flora_special_builtin_synonym/2,
	flora_op_synonym/2
   from flrsynonym.

:- import
	flratom_char_code/3
   from flora_ground.

:- import
	put_attr/3
   from machine.

:- import
	has_compiler_persistentoption/1
   from flrcompiler.

:- export
	flora_reset_composer/0,
	flora_composer_context/1,
	flora_set_composer_context/1,
	flora_clear_composer_context/1,
	flora_compose/3,
	flora_nth_token/2,
	flora_token_rulenum/2.

:- export
	is_infix/4,
	is_suffix/3,
	is_prefix/3.
