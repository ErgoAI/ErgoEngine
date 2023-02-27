## File:      ErgoAI/genincludes/Makefile
##
## Author(s): Michael Kifer
##            Guizhen Yang
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 1999-2023.
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

ALLOBJS = flrpatch.flh flrpreddef.flh flrindex_P.flh flrindex_static_P.flh \
	  flrdyndeclare.flh \
	  flrhead_dispatch.flh \
	  flrexportcheck.flh \
	  flrtable.flh flrtable_always.flh flrhilogtable.flh \
	  flrdefeasible.flh \
	  flropposes.flh \
	  flropposes_data.flh \
	  flrdynamic_connectors.flh \
	  flrrefreshtable.flh

PROLOG = none

PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

FLORA_INCLUDES_DIR = ../flrincludes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_TABLING_METHODS = $(FLORA_INCLUDES_DIR)/flora_tabling_methods.flh
FLORA_WSPREDICATES = $(FLORA_INCLUDES_DIR)/flora_wspredicates.flh

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_genincl_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/genincludes:"

# suffix rules
.SUFFIXES: .fli .flh

all: clean init $(ALLOBJS) finish
	@$(FLORADIR)/touch.sh cmd...
	$(PROLOG_COMMAND) < cmd...
	@rm cmd...
	@cat $(SVFILE)


%.flh: %.fli $(FLORA_TERMS) $(FLORA_WSPREDICATES)  $(FLORA_TABLING_METHODS)
	echo 'flWrapAround($*).' >> cmd...


init:
	/bin/rm -f $(SVFILE)
	@echo "add_lib_dir(a('..'))." >> cmd...
	@echo "['../flora2']." >> cmd...
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...
	@echo "'_#flmakesetup'." >> cmd...
	@echo "[flrutils]." >> cmd...
	@echo "[flrwraparound]." >> cmd...
	@echo "file_reopen($(SVFILE),w,$(STDWARN),_)." >> cmd...
	@echo "file_clone($(STDWARN),$(STDERR),_)." >> cmd...
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." >> cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN),_)." >> cmd...


finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


clean:
	/bin/rm -f *~ .*~ *.flh *.bak .#* cmd...


.DONE:
	@echo Done
