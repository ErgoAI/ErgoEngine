## File:      ErgoAI/lib/NMakefile.mak - Make file for Microsoft NMAKE
##
## Author(s): Michael Kifer
##            Guizhen Yang
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 1999-2018.
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

ALLOBJS = flrgclp$(PROLOGEXT) \
	  gclpe$(PROLOGEXT) \
	  ogclp$(PROLOGEXT) \
	  sgclp$(PROLOGEXT) \
	  ogclpe$(PROLOGEXT) \
	  atck2$(PROLOGEXT) \
	  atck3$(PROLOGEXT) \
	  atco$(PROLOGEXT) \
	  atco2$(PROLOGEXT) \
	  atco3$(PROLOGEXT) \
	  atck1alt$(PROLOGEXT)

OPTIONS = [optimize]

.SUFFIXES: $(PROLOGEXT) .flr .ergo

ALL: $(ALLOBJS)
	cd prolog
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)"
	cd ..

flrgclp$(PROLOGEXT): ..\flrincludes\flora_terms.flh
	"$(PROLOG)" -e "add_lib_dir(a('..')). ['..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup'. import flora_compile_system_module/1 from flrutils. flora_compile_system_module(%|fF). halt."

$(ALLOBJS): ..\flrincludes\flora_terms.flh
.flr$(PROLOGEXT):
	"$(PROLOG)" -e "add_lib_dir(a('..')). ['..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup'. import flora_compile_system_module/1 from flrutils. flora_compile_system_module(%|fF). halt."
.ergo$(PROLOGEXT):
	"$(PROLOG)" -e "add_lib_dir(a('..')). ['..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup'. import flora_compile_system_module/1 from flrutils. flora_compile_system_module(%|fF). halt."


CLEAN:
	-@if exist *$(PROLOGEXT) erase *$(PROLOGEXT)
	-@if exist *$(OBJEXT) erase *$(OBJEXT)
	-@if exist *.flh erase *.fdb
	-@if exist *.fld erase *.fld
	-@if exist *.flt erase *.flt
	-@if exist *.fls erase *.fls
	-@if exist *.fls2 erase *.fls2
	-@if exist *.flm erase *.flm
	-@if exist .flora_aux_files\*.xwam  del /q .flora_aux_files
	-@if exist .ergo_aux_files\*.xwam   del /q .ergo_aux_files
	-@if exist prolog\*.xwam  erase prolog\*.xwam
	-@if exist *~ erase *~
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*
