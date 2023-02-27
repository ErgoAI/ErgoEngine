## File:      ErgoAI/syslib/Makefile - Make file for Microsoft NMAKE
##
## Author(s): Michael Kifer
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

ALLOBJS =  flranswer$(OBJEXT) \
	   flrcontrol$(OBJEXT) \
	   flrclause$(OBJEXT) \
	   flrdisplay$(OBJEXT) \
	   flrload$(OBJEXT) \
	   flraggavg$(OBJEXT) \
	   flraggcolbag$(OBJEXT) \
	   flraggcolset$(OBJEXT) \
	   flraggcount$(OBJEXT) \
	   flraggmax$(OBJEXT) \
	   flraggmin$(OBJEXT) \
	   flraggsum$(OBJEXT) \
	   flraggsortby$(OBJEXT) \
	   flrstorageutils$(OBJEXT) \
	   flrstoragebase$(OBJEXT) \
	   flrdbop$(OBJEXT) \
	   flrbtdbop$(OBJEXT) \
	   flrshdirect$(OBJEXT) \
	   flrdynmod$(OBJEXT) \
	   flrdynrule$(OBJEXT) \
	   flrsemantics$(OBJEXT) \
	   flrnegation$(OBJEXT) \
	   flrtables$(OBJEXT) \
	   flrimportedcalls$(OBJEXT) \
	   flrerrhandler$(OBJEXT) \
	   flrdecode$(OBJEXT)  \
	   flrcanon$(OBJEXT)  \
	   flrmetaops$(OBJEXT)  \
	   flrtrim$(OBJEXT) \
	   flrtruthvalue$(OBJEXT) \
	   flrexport$(OBJEXT) \
	   flroptimize$(OBJEXT) \
	   flrnewoid$(OBJEXT) \
	   flrimport$(OBJEXT) \
	   flrhooks$(OBJEXT) \
	   flrcallflora$(OBJEXT) \
	   flrcommon_definitions$(OBJEXT) \
	   flrrulesigops$(OBJEXT) \
	   flrdescriptor_support$(OBJEXT) \
	   flrdelaysupport$(OBJEXT) \
	   flrhypothetical$(OBJEXT) \
	   flrcheckcard$(OBJEXT) \
	   flrudf$(OBJEXT) \
	   flrwhen$(OBJEXT) \
	   flrsetops$(OBJEXT) \
	   flrupdatecounter$(OBJEXT) \
	   flrcounter$(OBJEXT) \
	   flrencoding$(OBJEXT) \
	   flrlocalize$(OBJEXT) \
	   flrmodular$(OBJEXT) \
	   flrauxil$(OBJEXT) \
	   flrunify_handlers$(OBJEXT) \
	   flrtesting$(OBJEXT) \
	   flrgateway$(OBJEXT) \
	   flrchangemod$(OBJEXT) \
	   flrprefixdef$(OBJEXT)

OPTIONS = [optimize,ti_all,spec_repr]

.SUFFIXES: $(PROLOGEXT) $(OBJEXT)

ALL:: $(ALLOBJS)

CLEAN :
	-@if exist *~ erase *~
	-@if exist *$(OBJEXT) erase *$(OBJEXT)
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*


## %|fF as a file spec means: %|...F - file parts selection syntax. f- take
##                                     just the base name
$(PROLOGEXT)$(OBJEXT):
$(ALLOBJS): ..\flrincludes\flora_terms.flh
$(ALLOBJS): ..\flrincludes\flora_exceptions.flh
	"$(PROLOG)" -e "add_lib_dir(a('..')). ['..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup',mc(%|fF,$(OPTIONS)). halt."


