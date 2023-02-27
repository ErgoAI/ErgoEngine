## File:      ErgoAI/syslib/Makefile
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


OBJEXT=.xwam
PROLOGEXT = .P
PROLOGHEADEREXT=.H

PROLOG = none
FLORADIR = ..

ALLOBJS = flranswer$(OBJEXT) \
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
	  flrerrhandler$(OBJEXT)  \
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
	  flrauxil$(OBJEXT) \
	  flrsetops$(OBJEXT) \
	  flrupdatecounter$(OBJEXT) \
	  flrcounter$(OBJEXT) \
	  flrencoding$(OBJEXT) \
	  flrlocalize$(OBJEXT) \
	  flrmodular$(OBJEXT) \
	  flrunify_handlers$(OBJEXT) \
	  flrtesting$(OBJEXT) \
	  flrgateway$(OBJEXT) \
	  flrchangemod$(OBJEXT) \
	  flrprefixdef$(OBJEXT)

FLORA_INCLUDES_DIR = ../flrincludes
DYN_INCLUDES_DIR = ../includes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_ERRORS = $(FLORA_INCLUDES_DIR)/flora_errors.flh
FLORA_EXCEPTIONS = $(FLORA_INCLUDES_DIR)/flora_exceptions.flh
FLRHEADER = $(DYN_INCLUDES_DIR)/flrheader.flh


OPTIONS = [optimize,ti_all,spec_repr]

PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_syslib_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/syslib:"

.SUFFIXES: $(PROLOGEXT) $(PROLOGHEADEREXT) $(OBJEXT)

all: init $(ALLOBJS) finish
	./CompileChangedFiles.sh $(PROLOG_COMMAND)
	@cat $(SVFILE)


%$(OBJEXT): %$(PROLOGEXT) $(FLORA_TERMS) $(FLORA_EXCEPTIONS)
	@echo 'mc($*,$(OPTIONS)).' >> cmd...

%$(PROLOGEXT) : %$(PROLOGHEADEREXT)
	../touch.sh $@

flrcontrol$(OBJEXT) : flrcontrol$(PROLOGEXT) $(FLORA_TERMS) $(XSB_STANDARD)
	@echo 'mc(flrcontrol,$(OPTIONS)).' >> cmd...

flrdisplay$(OBJEXT) : flrdisplay$(PROLOGEXT) $(FLORA_TERMS) $(XSB_STANDARD)
	@echo 'mc(flrdisplay,$(OPTIONS)).' >> cmd...

flrimport$(OBJEXT): flrimport$(PROLOGEXT) $(FLORA_TERMS) $(FLRHEADER) $(FLORA_EXCEPTIONS)
	@echo 'mc(flrimport,$(OPTIONS)).' >> cmd...


init:
	/bin/rm -f $(SVFILE)
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." >> cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN),_)." >> cmd...
	@echo "add_lib_dir(a('..'))." >> cmd...hdr
	@echo "['../flora2']." >> cmd...hdr
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...hdr
	@echo "'_#flmakesetup'." >> cmd...hdr
	@echo "file_reopen($(SVFILE),a,$(STDWARN),_)." >> cmd...hdr
	@echo "file_clone($(STDWARN),$(STDERR),_)." >> cmd...hdr


finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


clean:
	/bin/rm -f core *~ .*~ *$(OBJEXT) *.O *.bak .#* cmd*


.DONE:
	@echo Done
