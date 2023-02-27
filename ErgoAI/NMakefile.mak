## File:      ErgoAI/NMakefile.mak -- Make file for Microsoft NMAKE
##
## Author(s): Michael Kifer
##
## Contact:   see  CONTACTS.txt
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 1999-2018;
##      and Vulcan, Inc., 2008-2013.
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

ALLOBJS = flrarguments$(OBJEXT) \
	  flrcomposer$(OBJEXT) \
	  flrcompiler$(OBJEXT) \
	  flrcoder$(OBJEXT) \
	  flrdependency$(OBJEXT) \
	  flrlexer$(OBJEXT) \
	  flrlibman$(OBJEXT) \
	  flrnodefp$(OBJEXT) \
	  flrnowsp$(OBJEXT) \
	  flroperator$(OBJEXT) \
	  flrparser$(OBJEXT) \
	  flrporting$(OBJEXT) \
	  flrpretifydump$(OBJEXT) \
	  flrprint$(OBJEXT) \
	  flrprolog$(OBJEXT) \
	  flrregistry$(OBJEXT) \
	  flrshell$(OBJEXT) \
	  flrshell_loop_handler$(OBJEXT) \
	  flrundefined$(OBJEXT) \
	  flrundefhook$(OBJEXT) \
	  flrutils$(OBJEXT) \
	  flrversion$(OBJEXT) \
	  flrwraparound$(OBJEXT) \
	  flrsynonym$(OBJEXT) \
	  flrsimpleprimitive$(OBJEXT) \
	  flrnosymbolcontext$(OBJEXT) \
	  flrnowarn$(OBJEXT) \
	  flrwrapper$(OBJEXT)

OPTIONS = [optimize,ti_all,spec_repr]

.SUFFIXES: $(PROLOGEXT) $(OBJEXT)

## cc is handled specially, by makeflora
ALL::  $(ALLOBJS)
	cd closure
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\includes
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\genincludes
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\syslib
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\datatypes
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\debugger
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\lib
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\AT
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\pkgs
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\demos
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\docs
	nmake /nologo /f NMakefile.mak copy
        cd ..
	-@if exist ergo_lib (  \
	     cd ergo_lib   &&  \
	     nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"  &&  \
	     cd ..   \
	)
	-@if exist ergo_pkgs (  \
	     cd ergo_pkgs   &&  \
	     nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"  &&  \
	     cd ..   \
	)
	-@if exist cc\ergoact\ergoact_check.P (  \
	     cd cc\ergoact &&  \
	     nmake /nologo /f NMakeProlog.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"  &&  \
	     cd ..\..   \
	)
	-@if exist cc\ergoflt\ergoflt_check.P (  \
	     cd cc\ergoflt &&  \
	     nmake /nologo /f NMakeProlog.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"  &&  \
	     cd ..\..   \
	)

TOP::  $(ALLOBJS)

CORE::  $(ALLOBJS)
	cd closure
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\includes
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\genincludes
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\syslib
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\datatypes
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..\debugger
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
	cd ..
	-@if exist cc\ergoact\ergoact_check.P (  \
	  cd cc\ergoact   &&  \
	  nmake /nologo /f NMakeProlog.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"  &&  \
	  cd ..\..   \
	)
	-@if exist cc\ergoflt\ergoflt_check.P (  \
	  cd cc\ergoflt   &&  \
	  nmake /nologo /f NMakeProlog.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"  &&  \
	  cd ..\..   \
	)


CLEAN : CLEANTEMP
	-@if exist *~ erase *~
	-@if exist *$(OBJEXT) erase *$(OBJEXT)
	-@if exist .flora_aux_files\* erase .flora_aux_files\*
	-@if exist *.bak erase *.bak
	-@if exist .ergo_paths.bat  del /AH .ergo_paths.bat
	-@if exist .#* erase .#*
	-@if exist flora2$(OBJEXT) erase flora2$(OBJEXT)
	cd cc
	nmake /nologo /f NMakefile.mak clean
	cd ..\closure
	nmake /nologo /f NMakefile.mak clean
	cd ..\genincludes
	nmake /nologo /f NMakefile.mak clean
	cd ..\includes
	nmake /nologo /f NMakefile.mak clean
	cd ..\datatypes
	nmake /nologo /f NMakefile.mak clean
	cd ..\syslib
	nmake /nologo /f NMakefile.mak clean
	cd ..\debugger
	nmake /nologo /f NMakefile.mak clean
	cd ..\lib
	nmake /nologo /f NMakefile.mak clean
	cd ..\AT
	nmake /nologo /f NMakefile.mak clean
	cd ..\pkgs
	nmake /nologo /f NMakefile.mak clean
	cd ..\demos
	nmake /nologo /f NMakefile.mak clean
	cd ..\docs
	nmake /nologo /f NMakefile.mak clean
	cd ..\emacs
	nmake /nologo /f NMakefile.mak clean
	cd ..
	-@if exist cc\ergoact\ergoact_check.P (  \
	  cd cc\ergoact   &&  \
	  nmake /nologo /f NMakeProlog.mak clean   &&  \
	  cd ..\..   \
	)
	-@if exist cc\ergoflt\ergoflt_check.P (  \
	  cd cc\ergoflt   &&  \
	  nmake /nologo /f NMakeProlog.mak clean   &&  \
	  cd ..\..   \
	)
	-@if exist ergo_lib\NMakefile.mak (  \
	  cd ergo_lib   &&  \
	  nmake /nologo /f NMakefile.mak clean   &&  \
	  cd ..   \
	)
	-@if exist ergo_pkgs\NMakefile.mak (  \
	  cd ergo_pkgs   &&  \
	  nmake /nologo /f NMakefile.mak clean   &&  \
	  cd ..   \
	) 


## %|fF as a file spec means: %|...F - file parts selection syntax. f- take
##                                     just the base name
$(PROLOGEXT)$(OBJEXT):
$(ALLOBJS): flrincludes\flora_terms.flh
$(ALLOBJS): flrincludes\flora_exceptions.flh
	"$(PROLOG)" -e "['.\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup',mc(%|fF,$(OPTIONS)). halt."


.SILENT:

CLEANTEMP :
	-@for /D %%i in (%USERPROFILE%\.xsb\"flora-*") do if exist %%i\*$(OBJEXT) erase %%i\*$(OBJEXT) %%i\*$(PROLOGEXT)
