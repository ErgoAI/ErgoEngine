/* File:      flrlexer.H
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


:- import
	append/3,
	memberchk/2
   from basics.

:- import parse_filename/4 from machine.

:- import
	flora_concat_atoms/2,
	flora_get_counter/2,
	flora_set_counter/2,
	flora_increment_counter/4
   from flrporting.

:- import flora_secondary_prompt/0 from flrprint.
:- import
	flratom_char_code/3
   from flora_ground.

:- import flora_builtin_synonym/2 from flrsynonym.

:- dynamic flora_file_stack/2.
:- dynamic flora_file_list/2.
:- dynamic flora_file_first_token/2.
:- dynamic flora_file_last_token/2.

:- index(flora_file_stack/2,trie).
:- index(flora_file_list/2,trie).


:- export
	flora_reset_lexer/0,
	flora_reset_lexer/1,
	flora_reset_charpos/0,
	flora_set_charpos/2,
	flora_get_charpos/2,
	flora_tokens/2,
	flora_token_text/6,
	flora_symbol_token_struct/6,
	flora_transactional_symbol_token_struct/6,
	flora_rule_delimiter_struct/6,
	flora_blank_line/1,
	flora_begin_file/1,
	flora_end_file/1,
        flora_compile_filename_by_token/2,
	flora_current_compile_filename/1,
	flora_current_compile_filename/2,
        is_whitespace/1,
	is_alpha/1,
	is_alpha_or_underscore/1,
	is_digit/1,
	is_alphanum/1,
	is_alphanum_or_underscore/1,
	strip_preprocessor_directive/2.
:- export
        lexer_waits_for_input/0,
        set_lexer_waits_for_input/0,
        reset_lexer_waits_for_input/0.
