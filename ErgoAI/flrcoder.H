/* File:      flrcoder.H
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

:- import format/2 from format.

:- import
	length/2,
	append/3
   from basics.

:- import term_to_atom/3 from string.

:- import standard_symbol/3 from std_xsb.

:- import
        psc_insert/4,
        term_new/2
   from machine.

:- import
	%%flora_abort/1,
	flora_abort/2,
	%%flora_term_to_atom_quote/2,
	flora_double_backslash_and_quotes/2,
	flora_double_backslash/2,
	flora_write_quoted_atom/1,
	flora_write_atom/1,
	flora_concat_atoms/2
   from flrporting.

%%:- import flratom_char_code/3 from flora_ground.

:- import flora_error_line/2 from flrprint.

:- import
	negate_wrapper/2,
	flora_module_predicate/4,
	flora_fdb_storage_name/2,
	flora_fdb_storage_name/2,
	flora_modularize_atom/3,
	%% Storage for skeletons of defined predicates
	flora_fld_storage_name/2
   from flrwrapper.

:- import
	flora_zap_attrvars/2
   from flrutils.

%% making is_flogic/4 dynamic and indexing as a trie makes the coder faster
:- dynamic is_flogic(_,_,_,_).
:- index(is_flogic/4,trie).

:- index(is_pratomlit/5,2).
:- index(is_prtermlit/5,2).
:- index(is_prtransactionaltermlit/5,2).
:- index(is_prtransactionalatomlit/5,2).


:- import flora_parse_datatype/4 from flrdatatype_parse.
:- import
	flora_special_datatype_internal_representation/2,
	flora_canonical_type_name/2
   from flrdatatype.
%%:- import flora_configuration/2 from flrregistry.

:- import
	no_errors_found/1
   from flrcompiler.

:- export
	flora_divide_program/3,
	flora_extern_code/2,
	flora_intern_code/4,
	flora_write_struct/3,
	flora_build_struct/7.

:- export
        '_$_$flrcoder_dummy_export'/0,
	flora_coder_write_prolog_bridge_rule/3,
	flora_coder_write_conditional_prolog_bridge_rule/2,
	is_prrule/3,
	is_prfact/2,
	is_prquery/2,
	is_prdirective/2,
	%%is_prlabeled_construct/3,
        is_prignoredep/2,
	is_prthismodule/1,
	is_prthisfdbstorage/1,
	is_prthisfldstorage/1,
        is_prmodularized_atom/3,
        is_prmodularized_atom/4,
	is_pratom/2,
	is_pratom/3,
	is_strict_pratom/3,
	is_prplgatom/4,
	is_prnumber/1,
	is_prnumber/3,
	is_prtransactionalnumber/3,
	is_prvariable/1,
	is_prvariable/3,
	is_prtransactionalvariable/3,
        is_pratomnumberstringbuiltin/2,
        is_prconjunct/3,
        is_prdisjunct/3,
	is_prstring/1,
	is_prstring/3,
        is_table_directive/2,
	is_prbuiltin_identifier/2,
	is_prbuiltin_identifier/3,
	is_prdatatype/4,
	is_prdatatypelist/1,
	is_prdatatypelist/2,
	is_prlist/4,
        is_prspecialvar/2,
        is_prcallervar/1,
        is_prdyncallervar/1,
        is_prfilenameinruleidvar/1,
	is_prfdbstorage/3,
        is_primport/4,
        is_prcmpopt/2,
	is_prterm/4,
	is_goallogic/4,
        is_prquantifier/5,
        is_prconstraint/2,
        is_prverbatim/2,
	is_prcall/2,
	is_pratomlit/5,
	is_prtransactionalatomlit/5,
	is_prtermlit/5,
	is_prtransactionaltermlit/5,
	is_prworkspace/3,
        is_prworkspaceterm/2,
	is_florasyslib/5,
	is_prologterm/3,
	is_prologterm/4,
	is_prologterm/5,
	is_prologliblit/4,
	is_prologliblit/5,
        is_prologatomlit/3,
        is_prologatomlit/4,
	is_prnewpred/3,
	is_prnewoid/3,
        is_prmvdcondef/6,
	is_primvdcondef/6,
        is_prneg/2,
        is_prrulelognaf/5,
	is_prreify/2,
	is_flogic/4,
	is_fllibdb/1,
	is_ruleupdate/1,
	is_prfloralib/3.
