/* File:      flrutils.H
**
** Author(s): Guizhen Yang
**
** Contact:   see  CONTACTS.txt
**
** Copyright (C) by
**      The Research Foundation of the State University of New York, 1999-2018;
**      and Vulcan, Inc., 2010-2013.
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
	del_attr/2,
        delete_attributes/1,
	term_psc/2,
	term_new/2,
        epoch_nanoseconds/2,
	parse_filename/4
   from machine.

:- import
	flora_abolish_nonincremental_tables/0
   from flrtables.

:- import
	flora_abort/1,
	flora_abort/2,
	flora_user_floradir/1,
	flora_file_op/2,
	flora_file_op/3,
	flora_module_path_get/1,
	flora_slash/1,
	flora_cputime/1,
	flora_walltime/1,
	flora_set_counter/2,
	flora_get_counter/2,
	flora_increment_counter/4,
	flora_concat_atoms/2,
	flora_concat_items/2,
	flora_match_substring/3,
	flora_copy_input/2,
	flora_double_backslash_and_quotes/2,
	flora_double_backslash/2,
	flora_write_atom/1,
	flora_delete_from_list/3,
	flora_set_banner_control/2,
        flora_get_user/1,
	flora_running_under/1
   from flrporting.

:- import
	flrterm_vars/3
   from flora_ground.

:- import
	length/2,
	flatten/2,
	memberchk/2,
	member/2,
	append/3
   from basics.
:- import
	last/2,
	append_lists/2
   from lists.

:- import
        file_time_ns/3,
	file_clone/3,
	file_open/3,
	copyIOport/2
   from file_io.

:- import
        remove_lib_dir/1,
        current_loaded_file/1
   from consult.

:- import xpp_options/1, xpp_process_file/3 from parse.

:- import process_control/2 from shell.

:- import xpp_process_file/3 from parse.
:- import xsb_configuration/2 from xsb_configuration.

:- import 
	flora_db_insert_base_fast/2,
	flora_db_commit/1,
	flora_db_reclaim_space/1,
	flora_db_delete_all/1,
	flora_storage_find_fact/2
   from flrstoragebase.

:- import
	storage_find_fact/2,
	storage_delete_fact/3,
	storage_insert_fact/3
   from storage.

:- import
        flora_raise_update_flag/0,
        flora_increment_update_counter/0
   from flrupdatecounter.

:- import
	incr_assert/1,
	incr_retractall/1
   from increval.

:- import flora_call_hooks/6 from flrhooks.

:- import
        add_flora_configuration_dir/2,
        has_flora_configuration_dir/2,
        flora_disable_keyboard_interrupts/0,
        flora_enable_keyboard_interrupts/0,
	'\\isloaded'/1
   from flora2.

:- import
	flora_reset_lexer/0,
	flora_reset_lexer/1,
	flora_tokens/2,
	flora_token_text/6,
	flora_set_charpos/2,
	flora_get_charpos/2,
	flora_symbol_token_struct/6,
	flora_rule_delimiter_struct/6,
	flora_blank_line/1,
	flora_current_compile_filename/1,
        flora_compile_filename_by_token/2,
	strip_preprocessor_directive/2
   from flrlexer.

:- import
	flora_set_composer_context/1,
	flora_clear_composer_context/1,
	flora_reset_composer/0,
	flora_compose/3,
	flora_nth_token/2
   from flrcomposer.

:- import flora_reset_operator/0 from flroperator.

:- import
	flora_user_modular_prlgdefs/2,
	flora_reset_modular_prolog/0,
	flora_reset_prolog/0
   from flrprolog.

:- import
	flora_reset_parser/0,
	FLORA_PROJECT_DEPENDENCY/2,
	flora_parse_term_external/3,
	flora_parse/3
   from flrparser.

:- import
        FLORA_INCLUDE_PRJ_INFO/1,
        FLORA_INCLUDE_PRJ_INFO/2
   from usermod.
:- dynamic
        FLORA_INCLUDE_PRJ_INFO/1,
        FLORA_INCLUDE_PRJ_INFO/2.

:- import FL_TEXTUAL_INFO_PRED/6 from usermod.
:- dynamic FL_TEXTUAL_INFO_PRED/6.

:- import
        FL_RULE_ENABLER/3,
        FL_TAG_DESCRIPTOR_META_PREDICATE/5,
        FL_BOOL_DESCRIPTOR_META_PREDICATE/5,
        FL_PROP_DESCRIPTOR_META_PREDICATE/6
   from usermod.

:- import
	flora_query_permitted_during_pause/0
   from usermod.

:- import
	flora_clear_compilation_permaoptions/0,
	flora_reset_compiler/1,
	flora_compile_pathexp/3,
	flora_compile/10,
        flora_compile_add/8,
	flora_get_compilation_liboptions/1,
	has_compiler_permaoption/1,
	has_compiler_persistentoption/1,
        flora_set_compilation_context/1,
        flora_clear_compilation_context/1,
	symbol/8,
	nowarn_symbol/5,
	set_turnoff_symbol_warnings_flag/1,
	clear_turnoff_symbol_warnings_flag/0,
	make_symbol_context_status/12,
	symbol_warnings_flag/1,
	compiling_error/3,
	flora_real_token_number/2,
	is_fldirective/3,
	is_flvariable/3
   from flrcompiler.

:- import
	is_prquery/2,
	is_prologliblit/4,
	is_prologliblit/5,
	flora_coder_write_prolog_bridge_rule/3,
	flora_coder_write_conditional_prolog_bridge_rule/2,
	flora_extern_code/2,
	flora_build_struct/7,
	flora_intern_code/4
   from flrcoder.

:- import 
	flora_debug_code/3
   from flrundefined.

:- import
	flora_configuration/2,
	flora_storage_registry/1,
	flora_trailer_registry/2,
	flora_used_udf_registry/4,
	flora_useudf_directive/5,
	flora_defined_udf_registry/6,
	flora_modular_prolog_registry/3,
	flora_defeasible_module_registry/2,
	flora_load_registry/7,
	flora_module_import_closure/2,
	flora_load_deps_closure/2,
	flora_imported_module_registry/2,
	flora_module_registry/1,
	flora_module_registry/2,
	flora_module_registry/3
   from flrregistry.

:- import
	flora_current_out_nl/0,
	flora_current_out_string/1,
	flora_current_out_string/2,
	flora_current_out_line/1,
	flora_current_out_line/2,

	flora_stderr_nl/0,
	flora_stderr_string/1,
	flora_stderr_string/2,

	flora_stdwarn_nl/0,
	flora_stdwarn_string/1,
	flora_stdwarn_string/2,
	
	flora_error_indentline/0,
	flora_warning_indentline/0,
	
	flora_error_line/1,
	flora_error_line/2,
	%%flora_error_line_noheader/1,
	flora_error_line_noheader/2,
	
	flora_warning_line/1,
	flora_warning_line/2,
	%%flora_warning_line_noheader/1,
	flora_warning_line_noheader/2,
	
	flora_loader_message/1,
	flora_loader_message/2,
	flora_loader_done_message/2,

	flora_loader_msg_nl/0,
	%%flora_stdmsg_nl/0,
	
	flora_send_abnormality_ctl_chars/1,
	%%flora_message_line/1,
	%%flora_message_line_noheader/1,
	%%flora_message_line_noheader/2,
        flora_stdfdbk_nl/0,
	flora_message_line/2
   from flrprint.

:- import
	flora_include/2,
	flora_load_system_module/1,
	flora_sysmod_file/3
   from flrlibman.

:- import
        enable_all_tripwires/0,
        disable_all_tripwires/0
   from flrerrhandler.

:- import P2H_PREDICATE/4 from prolog2hilog.

:- import prettyprint_stdin/0 from flrpretifydump.

:- import
	flora_modularize_atom/3,
	flora_check_module_name/1,
	valid_flora_module_name/1,
	flora_is_system_module_name/1,
	flora_fdb_storage_name/2,
	flora_module_name_error/1,
	%% support for undefinedness
	flora_fld_storage_name/2,
	flora_fls_storage_name/1,
	flora_fls_storage_name/2
   from flrwrapper.

:- import      
        flora_rename_vars_meaningfully/1,
	flora_decode_oid_as_atom/2,
	flora_write_fact/1,
	flora_write_rule_head/1,
	flora_write_oid/1,
	flora_write_rule_body/1
   from flrdecode.

:- import
	flora_attributed_vars_disabled/0,
	flora_put_attr/3
   from flrwhen.

:- import
	extract_stmt_fileinfo_from_filespec/4
   from flrdescriptor_support.

:- import
	flora_clear_runtime_uriprefixes/1
   from flrprefixdef.

:- import
	query_signature_member/3,
	matching_query_signature/4,
	rule_signature_member/3,
        matching_rule_signature/7
   from flrcanon.

:- import
	flora_answer_switch/1,
	flora_set_switch/1,
	flora_clear_switch/1
   from flrdebugger.

:- import check_for_dependencies/2 from flrdependency.

:- dynamic flora_xpp_extra_options/1.

:- dynamic flora_maxerr(_), flora_maxwarn(_).
:- index(flora_maxerr/1,0).
:- index(flora_maxwarn/1,0).

:- dynamic flora_compiler_environment(_,_).

:- dynamic flora_warnings_level(_).

:- export
	flora_set_maxerrors/1,
	flora_get_maxerrors/1,
	flora_set_maxwarnings/1,
	flora_get_maxwarnings/1,
	flora_retractall_substitute/1,

	flora_compile_util/1,
	flora_compile_util/2,
	flora_compile_system_module/1,

	flora_load_module_util/2,
	flora_load_module_util/3,
	flora_load_module_util/6,
        flora_save_module_util/1,
        flora_save_module_util/2,
	flora_load_modularized_prolog/2,
	
	flora_add_module_dyn/2,
	flora_add_module_dyn/3, 
	flora_add_module_dyn/6,
	flora_compile_add/2, 
	flora_compile_add/1,

	FL_DUMP/1,
	flora_locate_file/3,
	flora_locate_file_basic/2,
        flrfilename_error/3,

	flloadtrailer/2,
	fltrailersemantics/2,
	flora_end_of_input/1,
	flora_not_end_of_input/1,
	flora_shell_command_line/3,
	flora_check_filename/2,
	flora_set_xpp_options_for_compile/0,
	flora_set_xpp_options_for_compile/1,
	flora_set_xpp_options_for_compile/2,
	flora_set_xpp_options_for_read/0,
	flora_set_xpp_options_for_read/1,
	flora_set_xpp_options_for_read/2,
	flora_set_xpp_options_for_prolog_compile/1,
	flora_clear_xpp_options/0,
        flora_add_xpp_options/1,
	flora_replace_xpp_options/1,
	check_xpp_process_status/3,

	flora_copy_and_preprocess/3,
	flora_list2conjunct/2,

	flora_reclaim_storage_space/0,
	flora_commit_storage/0,
	flora_loaddyn_data/3,

        flora_tempfile_by_template/6,

	flora_compiler_environment/2,
	flora_cleanup_udf/1,
	flora_cleanup_registry/1,

	flora_experimenting_with/1,

	flora_read_canonical_and_insert/2,
	flora_read_symbols_canonical_and_insert/3,
	flora_read_descriptor_metafacts_canonical_and_insert/2,
	flora_load_system_module_util/1,
	flora_load_system_module_by_file/1,
	flora_measure_time/3,

	flora_read_then_parse/2,
	flora_read_then_parse/3,
	flora_read_then_parse_string/3,
	flora_read_then_parse_term_string/3,
	flora_read_then_parse_file/3,
	flora_read_then_parse_file/2,

	flora_read_parse_compile_build_term/3,
	flora_read_parse_compile_build_term/4,
	flora_read_parse_compile_build_term_list/3,
	flora_read_parse_compile_build_loop/3,
	flora_read_parse_compile_build_loop/4,
	flora_read_parse_compile_build_term_errwarn/4,
	flora_read_parse_compile_build_term_errwarn/5,

	util_read_parse_compile/11,
	util_read_parse_compile_add/9,

        update_load_registry/5,
	incremental_tabling_in_effect/1,
	flora_read_from_source/2,

	flora_token_index_to_line/2,
	flora_token_index_to_last_line/2,

        ergo_studio_ready/1,
        set_ergo_studio_state/1,

	merge_status_lists/3,

	reset_modules_for_shell/0,
	reset_modules_for_file/1,
	reset_modules_for_load/2,
	reset_modules_for_add/1,

	flora_get_textual_info/3,
	flora_zap_attrvars/1,
	flora_zap_attrvars/2,

        replace_flora_configuration/3,

        flora_system_info/1,

	flora_quietload_control/1,
	flora_warnings_level/1,
	flora_warnings_control/1,
	flora_set_warnings_control/1,
	flora_chatter_control/1,
	flora_feedback_control/1,
	flora_prompt_control/1,
        flora_coder_error/1.
