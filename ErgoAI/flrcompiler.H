/* File:      flrcompiler.H
**
** Author(s): Guizhen Yang
**
** Contact:   flora-users@lists.sourceforge.net
**
** Copyright (C) by
**      The Research Foundation of the State University of New York, 1999-2013;
**      and Vulcan, Inc., 2008-2013.
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

:- index atomlit_struct/3-2.
:- index termlit_struct/5-2.

:- import
	flora_increment_counter/4,
	flora_set_counter/2,
	flora_get_counter/2,
	flora_abort/2,
	%%flora_term_to_atom/2,
        flora_match_substring/3,
	flora_concat_atoms/2,
	flora_concat_items/2
    from flrporting.

:- import
	flora_op_synonym/2
   from flrsynonym.

:- import
	flrterm_vars_split/4,
	flratom_char_code/3
   from flora_ground.

:- import
	memberchk/2,
	member/2,
	subset/2,
	append/3,
	flatten/2,
	length/2
   from basics.

:- import
	put_attr/3,
	get_attr/3,
	del_attr/2,
	install_verify_attribute_handler/4,
	install_attribute_portray_hook/3
   from machine.

:- import
        flora_nonnegatable_system_module/1,
        flora_system_module/1
   from flrlibman.

:- import gensym/2 from gensym.

:- import
        %%concat_atom/2,
	substring/4
    from string.

:- import
        intersect/3,
        list_difference_eq/3,
	append_lists/2
   from lists.

:- import sort/2, keysort/2, findall/3 from setof.

:- import flora_define_operator/3 from flroperator.

:- import
	%%flora_define_prolog/1,
	%%flora_define_prolog/2,
	%%flora_define_prolog/3,
	flora_define_modular_prolog/2,
	flora_prlgdef_repl/4,
	flora_modular_prlgdef/2,
	flora_modular_prlgdef/3,
	flora_negatable_prlgdef/3,
	flora_prlgdef/3,
	flora_prlgdef/2
   from flrprolog.

:- import
	flora_clear_compiletime_uriprefixes/0,
	FLLIBPREFIXDEF/2
   from flrprefixdef.

:- import
	flora_current_compile_filename/1,
	flora_token_text/6
    from flrlexer.

:- import
	flora_token_rulenum/2,
	flora_nth_token/2
   from flrcomposer.

:- import
        list2commalist/2
   from flrauxil.
:- import
	flora_compiler_environment/2,
	flora_retractall_substitute/1,
	flora_token_index_to_line/2
   from flrutils.

:- import
	flora_intern_code/4,
	%%is_prscrambled_atom/3,
	%%is_prmodularized_atom/3,
	is_prologterm/3,
	is_prologterm/4,
	is_prologterm/5,
        is_prvariable/1,
        is_prvariable/3,
        is_prworkspace/3,
        is_prtransactionalvariable/3,
        is_prtransactionalnumber/3,
        is_pratomnumberstringbuiltin/2,
        is_pratom/2,
        is_pratom/3,
	is_strict_pratom/3,
	is_prplgatom/4,
        is_prnumber/3,
	is_prbuiltin_identifier/2,
	is_prbuiltin_identifier/3,
        is_prdatatype/4,
	is_prdatatypelist/2,
        is_prquantifier/5,
        is_prfact/2,
	is_prfloralib/3,
        is_prrulelognaf/5,
        is_prreify/2,
        is_prconjunct/3,
        is_prdisjunct/3,
        is_prlist/4,
	is_prterm/4
   from flrcoder.

:- import
	flora_modularize_atom/3,
	flora_scramble_atom/2
   from flrwrapper.

:- import
	flora_delayed_builtin_literal/2,
	flora_delayed_sensor_literal/3
   from FLDELAYSUPPORTMOD.
        
:- import flora_error_line/2 from flrprint.

:- import
	flora_get_flatomvar_name/2,
	flora_get_flatom_name/2,
	flatom_struct/3,
	flterm_struct/4,
	flconjunct_struct/3,
	%%fldisjunct_struct/3,
        %%flquantifier_struct/6,
	flthismodule_struct/1
   from flrparser.

:- import
        %%flora_configuration/2,
	flora_used_udf_registry/4
   from flrregistry.

:- import
	flora_reserved_builtin/2,
	flora_nodefp/2
   from flrnodefp.
:- import
	ignore_symbol_context/5
   from flrnosymbolcontext.

:- import
	flora_simple_primitive_extra_args/3,
	flora_simple_primitive/4
   from flrsimpleprimitive.

:- import
        flora_ignore_unsafe_vars/1
   from flrnowarn.

:- dynamic compile_with_var_module(_).

:- dynamic dynrule_warnings(_).

:- dynamic symbol/8.
:- index(symbol/8,trie).

:- export 
	flora_clear_compilation_permaoptions/0,
        flora_clear_compilation_persistentoptions/0,
	flora_compilation_context/1,
	flora_set_compilation_context/1,
	flora_clear_compilation_context/1,
	flora_get_compilation_liboptions/1,
        flora_reset_newvar/0,
	flora_reset_oid_related_counters/0,
        flora_new_varobj/3,
	flora_reset_compiler/1, 
	flora_compile_pathexp/3,
	flora_compile_add/8,
	flora_compile/10,
	flora_clear_compilation_symbols/0,
	has_compiler_permaoption/1,
	has_compiler_persistentoption/1,
	compiling_flrshell_command/0,
	symbol/8,
	nowarn_symbol/5,
	collect_flvars/2,
	collect_flvar_names/2,
	collect_prvar_names/2,
	remove_dupflvars/2,
	set_turnoff_symbol_warnings_flag/1,
	clear_turnoff_symbol_warnings_flag/0,
	make_symbol_context_status/12,
	symbol_warnings_flag/1,
	compiling_warning/3,
	compiling_error/3,
	set_production_mode/1,
	set_expert_mode/1,
        expert_mode_setting/1,
	set_omniform_mode/1,
        omniform_mode_setting/1,
	is_fltoken/3,
	is_fldirective/3,
	is_flvariable/3,
	is_flprefixdef/4,
	is_fltransactionalvariable/3,
        is_flfloralib/3,
        is_flfloralib/2,
        is_florasyslib_struct/5,
	approx_flindex/2,
	flora_real_token_number/2,
	no_errors_found/1.

%% these are eliminated by the precompiler
:- export
	is_flrule/3,
	is_fldynrule/4,
	is_flquery/2,
	is_fllatent_query/2,
	is_flfact/2,
        is_flatom/2,
        is_flatom/3,
        is_flbuiltin_identifier/2,
        is_flbuiltin_identifier/3,
        is_flnumber/2,
        is_flnumber/3,
	is_fludfdef/4,
	is_fludfdef_fact/3,
	is_flannotated_construct/3,
	is_flterm/4,
        is_fltransactionalterm/4,
	is_flobjref/4,
	is_fldelayedliteral/3,
	is_fldelayedliteral/4,
	is_flboolsig/4,
	%%is_flsetarg/3,
	is_fldescriptor/4,
	is_fltablerefresh/2,
	is_flpassthru/2,
	is_flhypothetical/4,
	is_flclause/3,
	is_flclause/4,
	is_fladd/2,
	is_flnewmodule/3,
	is_flcut/2,
	is_fltag_primitive/3,
	is_flp2h/3,
	is_flsigattconspec/6,
	is_flincattspec/5,
	is_flsimpleprimitive/3,
	is_flneg/3,
	is_flobjspec/4,
	is_flmvdattspec/5,
	is_flsigattspec/5,
	is_flinsert/4,
	is_flinsert/3,
	is_fldelayquant/5,
	is_fldelete/4,
	is_fldelete/3,
	is_flrulelognaf/2,
	is_flconstraint/2,
	is_fldatatype/4,
	is_flmetauniv/3,
	is_flstopdelay/3,
	is_flstopdelay/2,
	is_flplgnaf/2,
	is_flplib/2,
	is_flplib/3,
	is_flpliball/2,
	is_flpliball/3,
	is_flsigconstr/3,
	is_flprologifthen/3,
	is_fllogicquantifier/6,
	is_default_negation/3,
	is_flthrow/2,
	is_flcontrolconstruct/4,
	is_flload/2,
	is_flcallmetafacts/3,
	is_flconjunct/3,
	is_fldisjunct/3,
	is_flobjeql/3,
	is_flannotated_clause_stmt/4,
	is_flannotated_clause_stmt/5,
	is_flstring/2,
	is_flstring/3,
        is_flcharlist/3,
	is_flatomnumberstringbuiltin/1,
	is_fltransactionalstring/2,
	is_fltransactionalnumber/2,
	is_fltransactionalatom/2,
	is_fltoken/4,
	is_fltoken/3,
	is_fltoken/1,
	is_fltransactionaldatatype/4,
	is_flcallermodule/3,
	is_flarith/2,
        is_fllist/4,
	is_flcatch/4,
	is_flifthenelse/4,
	is_fltolistattspec/5,
	is_flupdaterule/3,
	is_fltrans_atomnumberstring/1,
	is_fltransig/4,
	is_flifthen/3,
	is_flerasemodule/3,
	is_fltruthvalue/3,
	is_flbirelate/4,
	is_reifyop/2,
	is_flprefixprimitive/4,
	is_flruleworkspace/3,
	is_flworkspace/3,
	is_fluniveqform/3,
	is_fltranspec/3,
	is_flmethspec/3,
        is_modularized_pratom/3,
	is_flaggregate/6.

:- export
        detach_flworkspace/3,
        extract_goal_from_prrulelognaf/2,
        %% compiler silencer
        '_$_$flrcompiler_dummy_export'/0.

:- export compile_useudf_directive/3.

:- export
	compile_reifyop/4,
	flora_flvar_diff/3,
	subtract_vars/3,
        flora_hypothetical_lib/1,
        flora_aggregate_lib/1,
        flora_aggregate_lib/2,
	flora_singleton_warnings/2.
