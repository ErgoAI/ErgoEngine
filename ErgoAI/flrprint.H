/* File:      flrprint.H
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
	format/2,
	format/3
   from format.

:- import
        flora_running_as/1,
	flora_abort/2,
	flora_get_counter/2,
	flora_banner_control/1
   from flrporting.

:- import file_flush/2 from file_io.

:- import file_put_code/2 from file_io.
:- import flora_configuration/2 from flrregistry.

:- import set_ergo_studio_state/1 from flrutils.


:- export
	flora_current_out_nl/0,
	flora_current_out_string/1,
	flora_current_out_string/2,
	flora_current_out_line/1,
	flora_current_out_line/2,

	flora_stream_out_nl/1,
	flora_stream_out_string/2,
	flora_stream_out_string/3,
	flora_stream_out_line/2,
	flora_stream_out_line/3,

	flora_stderr_nl/0,
	flora_stderr_string/1,
	flora_stderr_string/2,

	flora_stdwarn_nl/0,
	flora_stdwarn_string/1,
	flora_stdwarn_string/2,
	
	flora_stdmsg_nl/0,
	flora_stdmsg_string/1,
	flora_stdmsg_string/2,
	flora_stdmsg_line/1,
	flora_stdmsg_line/2,
	flora_loader_done_message/2,
	
	flora_stdfdbk_nl/0,
	flora_stdfdbk_string/1,
	flora_stdfdbk_string/2,
	flora_stdfdbk_line/1,

	flora_prompt_string/1,
	
	flora_error_heading/0,
	flora_error_indentline/0,
	flora_warning_heading/0,
	flora_warning_indentline/0,
	
	flora_error_line/1,
	flora_error_line/2,
	flora_error_line_noheader/1,
	flora_error_line_noheader/2,
	
	flora_abort_line/1,
	flora_abort_line/2,
	
	flora_warning_line/1,
	flora_warning_line/2,
	flora_warning_line_noheader/1,
	flora_warning_line_noheader/2,
        flora_warning_string/1,
	
	flora_message_line/1,
	flora_message_line/2,
	flora_message_line_noheader/1,
	flora_message_line_noheader/2,

	flora_loader_message/1,
	flora_loader_message/2,
	flora_loader_msg_nl/0,
	
	flora_send_abnormality_ctl_chars/1,
	flora_err_stream/1,
	flora_warn_stream/1,
	flora_msg_stream/1,
	flora_fdbk_stream/1.

:- export
        flora_secondary_prompt/0,
        flora_status_msg/1,
        flora_error_writeln/1, flora_error_writeln_noheader/1,
        flora_abort_writeln/1,
        flora_warning_writeln/1, flora_warning_writeln_noheader/1,
        flora_message_writeln/1, flora_message_writeln_noheader/1,
        flora_stdfdbk_writeln/1.
