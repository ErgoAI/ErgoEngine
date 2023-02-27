/* File:      flroperator.H
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


:- dynamic flora_dynop(_,_,_).
:- dynamic flora_shop(_,_,_).

:- index(flora_dynop/3,3).
:- index(flora_shop/3,3).
:- index(flora_op/3,3).

:- import flora_composer_context/1 from flrcomposer.

:- export
	flora_opdef/3,
	flora_parenthesized_op/1,
	flora_conflict_operator/3,
	flora_reset_operator/0,
	flora_define_operator/3,
	flora_shreset_operator/0,
	flora_shdefine_operator/3.
