/* File:      flrwrapper.H
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
#include "flora_terms.flh"
#include "flora_porting.flh"


:- import
	memberchk/2,
	length/2
   from basics.

:- import
	flora_abort/1,
	%%flora_abort/2,
	flora_term_to_atom/2,
	flora_match_substring/3,
	flora_match_substring/5,
	flora_get_substring/4,
	flora_concat_atoms/2
    from flrporting.

:- import
	flora_add_last/3,
	flora_generalize_last/2
   from flrtrim.

:- import term_psc/2, term_new/2, psc_name/2 from machine.

:- import
	flora_modular_prolog_registry/3,
	flora_storage_registry/1,
	flora_fld_storage_registry/1,
	flora_user_module_registry/1,
	flora_module_registry/1
   from flrregistry.

:- import
	%%FLORA_DELAY_CHECKER/2,
        FLLIBMODOBJ/2
   from usermod.


:- import flratom_char_code/3 from flora_ground.
:- import FLORA_NAF_PREDICATE/4 from flrnegation.
:- import get_self_or_delayed_call/2 from flrdelaysupport.


:- dynamic flora_flogic_fact_wrapper(_,_).
:- dynamic flora_current_flogic_module(_).
:- dynamic flora_encoded_call_cache(_,_,_,_).
:- dynamic flora_decoded_predicate_cache(_,_,_,_,_,_,_).
:- dynamic functor_arity_cache(_,_).
:- dynamic valid_flora_module_name_cache/1.

:- index(functor_arity_cache/2,[1,2]).

%% These indices do not seem to make a difference and can even hurt
%%:- index(flora_encoded_call_cache/4,[1,3,2]).
%%:- index(flora_decoded_predicate_cache/8,[1]).

:- index(flora_flogic_fact_wrapper/2,[1,2]).

:- export
	flora_module_predicate/4,
	flora_module_predicate_symbol/3,
	flora_hilog_module_predicate_symbol/2,
	flora_tabled_hilog_module_predicate_symbol/2,
	flora_dyna_hilog_module_predicate_symbol/2,
	flora_dynz_hilog_module_predicate_symbol/2,
	flora_tdyna_hilog_module_predicate_symbol/2,
	flora_tdynz_hilog_module_predicate_symbol/2,

	flora_fdb_storage_name/2,
	flora_fld_storage_name/2,
	flora_fls_storage_name/2,
	flora_fls_storage_name/1,
	flora_is_flogic_base/1,
	flora_storage_module/2,

	flora_setup_flogic_fact_wrapper/1,
	flora_flogic_fact_wrapper/2,

	is_flora_callable_literal/1,
	flora_decode_predicate/6,
	flora_decode_predicate/7,
	flora_decode_module_name/2,
	flora_decode_module_prefix/1,
	flora_get_symbol_module/2,
	flora_get_symbol_module_wrapper/3,
	flora_is_flogic_wrapper/3,
	flora_is_modularized_prolog/3,

	flora_is_fdb_storage/1,
	flora_is_fld_storage/1,

	flora_canonic_wrapper_symbol/2,
	negate_wrapper/2,
	negate_predicate/2,
	unnegate_wrapper/2,
	unnegate_predicate/2,
	flora_is_system_module_name/1,
	flora_check_module_name/1,
	valid_flora_module_name/1,
	flora_module_name_error/1,
	flora_change_module/5,

	flora_scramble_atom/2,
	flora_unscramble_atom/2,
	flora_modularize_atom/3,

	flora_is_transactional_wrapper/1,
	flora_is_transactional_call/1,
	flora_is_neg_wrapper/1,
        flora_is_neg_negated_pred/1,
	flora_funct_arity/2.

:- export flora_change_module_tabled_unnumber_call/4.

%% There is a conflict in the way "as" is defined in XSB and FLORA-2, so
%% XSB gives an error if we don't put parens around.
%% XSB might change its definition and then we'll be ok.
:- table
	(flora_storage_module/2,
	flora_is_system_module_name/1,
	flora_fdb_storage_name/2,
	flora_fld_storage_name/2)
   as opaque.
:- table
	(flora_is_neg_wrapper/1,
	negate_wrapper/2,
	flora_canonic_wrapper_symbol/2)
   as opaque.
