## File:      ErgoAI/closure/Makefile
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



FLORADIR = ..

ALLOBJS = flrcommon.flh \
	flrcommon_inh.flh \
	flrnoeqltrailer.flh \
	flrnoeqltrailer_inh.flh \
	flrnoeqltrailer_common.flh \
	flrtrailer_mono_inh.flh \
	flreqltrailer.flh \
	flreqltrailer_inh.flh \
	flreqltrailer_common.flh \
	flrscalareql.flh \
	flrpredeql.flh \
	flrimportaxioms.flh \
	flrprednoeql.flh

FLORA_INCLUDES_DIR = ../flrincludes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_TABLING_METHODS = $(FLORA_INCLUDES_DIR)/flora_tabling_methods.flh

PROLOG = none

PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

FLORA_PRED_DEF = ../flrincludes/flora_wspredicates.flh

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_closure_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/closure:"

# suffix rules
.SUFFIXES: .flh .fli

all: clean init $(ALLOBJS) finish
	@../touch.sh cmd...
	$(PROLOG_COMMAND) < cmd...
	@rm cmd...
	@cat $(SVFILE)


%.flh: %.fli $(FLORA_PRED_DEF)  $(FLORA_TERMS) $(FLORA_TABLING_METHODS)
	@echo 'flWrapAround($*).' >> cmd...


init:
	/bin/rm -f $(SVFILE)
	@echo "add_lib_dir(a('..'))." >> cmd...
	@echo "['../flora2']." >> cmd...
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...
	@echo "'_#flmakesetup'." >> cmd...
	@echo "import flWrapAround/1 from flrwraparound." >> cmd...
	@echo "file_reopen($(SVFILE),w,$(STDWARN),_)." >> cmd...
	@echo "file_clone($(STDWARN),$(STDERR),_)." >> cmd...
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." >> cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN),_)." >> cmd...


finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


clean:
	/bin/rm -f *~ .*~ *.flh *.bak .#* cmd...
	/bin/rm -f includes/*~


.DONE:
	@echo Done
