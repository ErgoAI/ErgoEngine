/* File:      flrimport.H
**
** Author(s): Michael Kifer
**
** Contact:   flora-users@lists.sourceforge.net
**
** Copyright (C) by
**      The Research Foundation of the State University of New York, 1999-2013
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
	copy_term/2
   from basics.
:- import flora_load_module_util/3 from flrutils.
:- import 
        flora_hilog_module_predicate_symbol/2,
        flora_tabled_hilog_module_predicate_symbol/2
    from flrwrapper.
:- import flora_error_line/1, flora_error_line/2 from flrprint.

:- export
	flora_import_predicate/4.



