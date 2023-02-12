## File:      ErgoAI/Makefile
##
## Author(s): Michael Kifer
##            Guizhen Yang
##
## Contact:   flora-users@lists.sourceforge.net
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 1999-2013;
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



OBJEXT=.xwam
PROLOGEXT=.P
PROLOGHEADEREXT=.H

FLORADIR = ../..

ALLOBJS = atco_init${OBJEXT}

FLORA_INCLUDES_DIR = $(FLORADIR)/flrincludes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_PORTING = $(FLORA_INCLUDES_DIR)/flora_porting.flh
FLORA_EXTENSIONS = $(FLORA_INCLUDES_DIR)/flora_extensions.flh
FLORA_WRAP = $(FLORA_INCLUDES_DIR)/flora_wrap.flh

OPTION = [optimize,ti_all,spec_repr]
#OPTION = [optimize]

# PROLOG is passed as an argument to make. Default is none, to trigger an error
PROLOG=none
PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_AT_prolog_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling  .../ErgoAI/AT/prolog:"

.SUFFIXES: $(PROLOGEXT) $(PROLOGHEADEREXT) $(OBJEXT)


# acceptable targets are all (default), system, base, top
all: init $(ALLOBJS) finish
	./CompileChangedFiles.sh $(PROLOG_COMMAND)
	@cat $(SVFILE)


%$(OBJEXT) : %$(PROLOGEXT) $(FLORA_TERMS) $(FLORA_PORTING) $(FLORA_EXTENSIONS) $(FLORA_WRAP)
	@echo 'mc($*,$(OPTION)).' >> cmd...



init:
	/bin/rm -f $(SVFILE)
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." > cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN),_)." >> cmd...
	@echo "add_lib_dir(a('../..'))." >> cmd...hdr
	@echo "['../../flora2']." >> cmd...hdr
	@echo "retractall(xpp_options(_))." >> cmd...hdr
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...hdr
	@echo "'_#flmakesetup'." >> cmd...hdr
	@echo "file_reopen($(SVFILE),a,$(STDWARN),_)." >> cmd...hdr
	@echo "file_clone($(STDWARN),$(STDERR),_)." >> cmd...hdr


finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


clean:
	/bin/rm -f core *~ .*~ *$(OBJEXT) *.O *.bak .#* cmd* *$(OBJEXT)


.DONE:
	@echo Done
