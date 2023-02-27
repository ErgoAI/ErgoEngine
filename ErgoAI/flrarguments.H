/* File:      flrarguments.H
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

:- dynamic flora_arguments_spec_dyn/4.

:- import
	flora_get_module_from_spec/3,
	flora_get_flatomvar_name/2
   from flrparser.

:- export
	flora_set_argdef/4,
	flora_prim_argdef/3,
	flora_argdef/4.
