## File:      ErgoAI/includes/NMakefile.mak - Make file for Microsoft NMAKE
##
## Author(s): Michael Kifer
##
## Contact:   flora-users@lists.sourceforge.net
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 2013-2018.
##      and Vulcan, Inc., 2013.
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
##



ALLOBJS = flrdefinition.flh

.SUFFIXES: .fli .flh

ALL: $(ALLOBJS)

CLEAN :
	-@if exist *~ erase *~
	-@if exist flrdefinition.flh erase flrdefinition.flh
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*


## %|fF as a file spec means: %|...F - file parts selection syntax. f- take
##                                     just the base name
$(ALLOBJS): ..\flrincludes\flora_terms.flh
.fli.flh:
	"$(PROLOG)" -e "add_lib_dir(a('..')). ['..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup'. [flrwraparound]. import flWrapAround/1 from flrwraparound. flWrapAround(%|fF). halt." 
