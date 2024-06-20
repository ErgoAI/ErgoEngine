## File:      ErgoAI/AT/Makefile
## Author(s): kifer
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

FLORADIR=..

ALL = flrgclp$(PROLOGEXT) \
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

Option = [optimize]


#-----------------------------------------------------------------------#
#
# Self-make: Version 1.4.1
#
PROLOG = none
PROLOG_COMMAND = "$(PROLOG)" -i -m 80000 -c 10000 -e 'segfault_handler(warn).'
#
#-----------------------------------------------------------------------#

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_AT_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/AT:"


# suffix rules
.SUFFIXES: ${PROLOGEXT} .flr .ergo

all: clean init recompile
	cd prolog && $(MAKE) PROLOG=$(PROLOG)

recompile: $(ALL) finish
	@../touch.sh cmd...
	$(PROLOG_COMMAND) < cmd...
	@rm cmd...
	@cat $(SVFILE)

# System ATs, like flrgclp, should be treated here
flrgclp${PROLOGEXT}:
	echo 'flora_load_system_module_by_file($*).' >> cmd...

# the default case
.flr${PROLOGEXT}:
	echo "import ('\\\\compile')/1 from flora2. '\\\\compile'($*)." >> cmd...
.ergo${PROLOGEXT}:
	echo "import ('\\\\compile')/1 from flora2. '\\\\compile'($*)." >> cmd...

init:
	rm -f $(SVFILE)
	@echo "add_lib_dir('..')." >> cmd...
	@echo "['../flora2']." >> cmd...
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...
	@echo "'_#flmakesetup'." >> cmd...
	@echo "import flora_load_system_module_by_file/1 from flrutils." >> cmd...
	@echo "file_reopen($(SVFILE),w,$(STDWARN),_)." >> cmd...
	@echo "file_clone($(STDWARN), $(STDERR),_)." >> cmd...
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." >> cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN), _)." >> cmd...

finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


.DONE:
	echo Done

clean:
	/bin/rm -f *${PROLOGEXT} *$(OBJEXT)  *~ .*~ *.bak .#* cmd*
	/bin/rm -rf .flora_aux_files prolog/*.xwam
	/bin/rm -rf .ergo_aux_files  prolog/*.xwam
	cd prolog && make clean
