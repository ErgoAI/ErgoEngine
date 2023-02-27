/* File:      flrshell.P  -- The Flora interactive shell
**
** Author(s): Guizhen Yang
**            Michael Kifer
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
#include "flora_terms.flh"


:- import
	push_abort_cutpoint/0,
	pop_abort_cutpoint/0
   from standard.

:- import
	memberchk/2,
	member/2
   from basics.

:- import
	current_prolog_flag/2
   from curr_sym.

:- import cwd/1, cd/1 from shell.
:- import call_xsb_hook/2, xsb_exit_hook/1 from xsb_hook.

:- import
	flora_configuration/2
   from flrregistry.

:- import
	flora_slash/1,
	flora_concat_atoms/2,

	flora_get_counter/2,
	flora_set_counter/2,
	flora_increment_counter/4,

	flora_module_path_get/1,
	flora_module_path_add/1,
	flora_module_path_removeall/1,
	flora_set_banner_control/2,
	flora_banner_control/1
   from flrporting.

:- import path_sysop/2 from file_io.

:- import flora_call_hooks/6 from flrhooks.

:- import pop_stdin/1 from push_io.

:- import
	flora_abolish_nonincremental_tables/0
   from flrtables.

:- dynamic  FL_STARTUP_MSG/1.
:- import
	%%FL_QUERY_PREDICATE0/1
	FL_STARTUP_MSG/1
   from usermod.

%%:- import close_open_tables/1 from machine.

:- import flora_undefined_predicate_msg/4 from flrundefhook.

:- import
        FL_LOAD(File),
	flora_get_break_level/1,
	flora_protected_break_level/0,
	flora_unprotect_break_level/0,
	unstrap_flora/0
   from flora2.

:- import flora_shreset_operator/0 from flroperator.

:- import flora_clear_runtime_uriprefixes/0 from flrprefixdef.

:- import
        reset_lexer_waits_for_input/0
   from flrlexer.

:- import
	set_production_mode/1,
	set_expert_mode/1,
	flora_reset_oid_related_counters/0
   from flrcompiler.

:- import
	flora_load_library/1,
	flora_load_system_module/1
   from flrlibman.

:- import
        flora_status_msg/1,
	flora_stdmsg_nl/0,
	flora_stdmsg_line/1,
	flora_stdmsg_line/2,
	flora_stdmsg_string/2,
	flora_message_line/1,
	flora_stdfdbk_line/0,
	flora_stdfdbk_string/1,
	flora_prompt_string/1,
	flora_stderr_nl/0,
	flora_stdwarn_nl/0,
	flora_warning_line/1,
	flora_error_line/1
   from flrprint.

:- import
        set_ergo_studio_state/1,
	flloadtrailer/2,
	fltrailersemantics/2,
	flora_shell_command_line/3,
	flora_load_module_util/2,
	flora_end_of_input/1,
	flora_check_filename/2,
	flora_chatter_control/1,
	flora_warnings_level/1
   from flrutils.

:- import flora_good_command/1 from flrcallflora.

:- import
	flora_answer_switch/1,
	flora_trace/0,
	flora_trace/1,
	flora_trace/2,
	flora_tracelow/0,
	flora_tracelow/1,
	flora_tracelow/2,
	flora_traceverylow/0,
	flora_traceverylow/1,
	flora_traceverylow/2,
	flora_notrace/0,
	flora_set_switch/1,
	flora_clear_switch/1,
	flora_logforestlow_start/0,
	flora_logforest_start/0,
	flora_logforestlow_start/1,
	flora_logforest_start/1,
	flora_logforestlow_start/2,
	flora_logforest_start/2,
	flora_logforest_end/0
   from flrdebugger.

:- import
        flora_shell_loop_handler/1
   from flrshell_loop_handler.


:- dynamic flora_warnings_level(_).

