## File:      ErgoAI/demos/Makefile
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 1999-2013.
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

ALL = 	aggregate${PROLOGEXT} benchmark${PROLOGEXT} \
	clpdemo${PROLOGEXT} default${PROLOGEXT} family_obj${PROLOGEXT} \
	family_rel${PROLOGEXT} flogic_basics${PROLOGEXT}\
	metavar${PROLOGEXT} mix${PROLOGEXT} module1${PROLOGEXT}\
	mod1${PROLOGEXT} \
	mono_inherit${PROLOGEXT} rel_ops${PROLOGEXT}\
	tree_traversal${PROLOGEXT} diamond${PROLOGEXT}\
	btupdates${PROLOGEXT} pretty${PROLOGEXT}

Option = [optimize]


#-----------------------------------------------------------------------#
#
# Self-make: Version 1.4.1
#
PROLOG = none
PROLOG_COMMAND = "$(PROLOG)" -i -m 60000 -c 10000 -e 'segfault_handler(warn).'
#
#-----------------------------------------------------------------------#

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_demos_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/demos:"


# suffix rules
.SUFFIXES: ${PROLOGEXT} .flr .ergo

all: clean init recompile

recompile: $(ALL) finish
	./CompileChangedFiles.sh $(PROLOG_COMMAND)
	@cat $(SVFILE)

.flr${PROLOGEXT}:
	echo "import ('\\\\compile')/1 from flora2. '\\\\compile'($*)." >> cmd...
.ergo${PROLOGEXT}:
	echo "import ('\\\\compile')/1 from flora2. '\\\\compile'($*)." >> cmd...

init:
	rm -f $(SVFILE)
	@echo Initializing...
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." >> cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN), _)." >> cmd...
	@echo "add_lib_dir(a('..'))." >> cmd...hdr
	@echo "['../flora2']." >> cmd...hdr
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...hdr
	@echo "'_#flmakesetup'." >> cmd...hdr
	@echo "file_reopen($(SVFILE),a,$(STDWARN),_)." >> cmd...hdr
	@echo "file_clone($(STDWARN), $(STDERR),_)." >> cmd...hdr

finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


.DONE:
	echo Done

clean:
	/bin/rm -f *${PROLOGEXT} *${PROLOGEXT}_gpp *${OBJEXT} *.O *.bak *.fdb *.fld *.flt *.fls *.fls *.flm *.con *~ .*~ .#* cmd*
	/bin/rm -f .flora_aux_files/*
	/bin/rm -f .ergo_aux_files/*
