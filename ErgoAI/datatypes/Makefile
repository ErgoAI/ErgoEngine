## File:      ErgoAI/datatypes/Makefile
##
## Author(s): Michael Kifer
##
## Contact:   flora-users@lists.sourceforge.net
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

ALLOBJS = flrdatatype$(OBJEXT) \
	flrdtype_sub$(OBJEXT) \
	flrdatatype_parse$(OBJEXT) \
	flrtime_arith$(OBJEXT) \
	flrcurrency$(OBJEXT) \
	flrdatatype_utils$(OBJEXT)

FLORA_INCLUDES_DIR = ../flrincludes
DYN_INCLUDES_DIR = ../includes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_ERRORS = $(FLORA_INCLUDES_DIR)/flora_errors.flh
FLRHEADER = $(DYN_INCLUDES_DIR)/flrheader.flh


OPTION = [optimize]

PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_datatypes_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/datatypes:"

.SUFFIXES: $(PROLOGEXT) $(PROLOGHEADEREXT) $(OBJEXT)

all: init $(ALLOBJS) finish
	./CompileChangedFiles.sh $(PROLOG_COMMAND)
	@cat $(SVFILE)


%$(OBJEXT): %$(PROLOGEXT) $(FLORA_TERMS)
	@echo 'mc($*,$(OPTION)).' >> cmd...

%$(PROLOGEXT) : %$(PROLOGHEADEREXT)
	../touch.sh $@


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
	/bin/rm -f core *~ .*~ *$(OBJEXT) *.O *.bak .#*


.DONE:
	@echo Done
