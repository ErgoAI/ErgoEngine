## File:      ErgoAI/debugger/Makefile
##
## Author(s): Michael Kifer
##            Guizhen Yang
##
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
PROLOGEXT=.P

FLORADIR = ..

ALLOBJS = flrdebugger${OBJEXT} \
	flrtabledump${OBJEXT} \
	flrterminyzer${OBJEXT} \
	dynamic_data.dat \
	static_data.dat

FLORA_INCLUDES_DIR = ../flrincludes
DYN_INCLUDES_DIR = ../includes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_ERRORS = $(FLORA_INCLUDES_DIR)/flora_errors.flh
FLRHEADER = $(DYN_INCLUDES_DIR)/flrheader.flh

.SUFFIXES:  .in .dat ${PROLOGEXT} .H ${OBJEXT}


OPTION = [optimize]

PROLOG = none

PROLOG_COMMAND = "$(PROLOG)" -m 2000 -c 2000 -e 'segfault_handler(warn).'

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_debug_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/debugger:"


all: init $(ALLOBJS) finish
	@../touch.sh cmd...
	$(PROLOG_COMMAND) < cmd...
	/bin/rm -f cmd...
	@cat $(SVFILE)

static_data.dat: static_data.in $(FLORA_TERMS)
	cp -f static_data.in static_data.dat

%.dat: %.in $(FLORA_TERMS)
	@echo 'flWrapAround($*).' >> cmd...


%$(OBJEXT) : %$(PROLOGEXT) $(FLORA_TERMS) $(FLORA_PORTING) $(FLORA_EXTENSIONS)
	@echo 'mc($*,$(OPTION)).' >> cmd...


init:
	/bin/rm -f $(SVFILE)
	@echo "add_lib_dir(a('..'))." >> cmd...
	@echo "['../flora2']." >> cmd...
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...
	@echo "'_#flmakesetup'." >> cmd...
	@echo "[flrwraparound]." >> cmd...
	@echo "file_reopen($(SVFILE),a,$(STDWARN),_)." >> cmd...
	@echo "file_clone($(STDWARN),$(STDERR),_)." >> cmd...
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." >> cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN),_)." >> cmd...


finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


clean:
	/bin/rm -f *~ .*~ *$(OBJEXT) *.O *.dat *.bak .#*


.DONE:
	@echo Done
