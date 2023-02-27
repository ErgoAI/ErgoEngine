## File:      ErgoAI/pkgs/prolog/NMakefile.mak -- Make file for Microsoft NMAKE
##
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 2016;
##      and Vulcan, Inc., 2016.
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

OBJEXT = .xwam
PROLOGEXT = .P

ALLOBJS = flrpm$(OBJEXT) \
	  flrxml$(OBJEXT)

OPTIONS = [optimize,ti_all,spec_repr]

.SUFFIXES: $(PROLOGEXT) $(OBJEXT)

ALL::  $(ALLOBJS)

$(PROLOGEXT)$(OBJEXT):
	"$(PROLOG)" -e "add_lib_dir(a('..\\..')). ['..\\..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup',mc(%|fF,$(OPTIONS)). halt."

CLEAN :
	-@if exist *~ erase *~
	-@if exist *$(OBJEXT) erase *$(OBJEXT)
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*

.SILENT:
