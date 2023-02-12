## File:      ergo_lib/ergo2java/Makefile
##
## Author(s): Michael Kifer
##
## Contact:   michael.kifer@coherentknowledge.com
##
## Copyright (C) Coherent Knowledge Systems, Inc., 2015
##
##



OBJEXT=.xwam
PROLOGEXT=.P
PROLOGHEADEREXT=.H

FLORADIR = ../..

ALLOBJS = interprolog${OBJEXT} \
	  ergo_java${OBJEXT} 

FLORA_INCLUDES_DIR = $(FLORADIR)/flrincludes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_PORTING = $(FLORA_INCLUDES_DIR)/flora_porting.flh
FLORA_EXTENSIONS = $(FLORA_INCLUDES_DIR)/flora_extensions.flh
FLORA_WRAP = $(FLORA_INCLUDES_DIR)/flora_wrap.flh

OPTION = [optimize,ti_all,spec_repr]

# PROLOG is passed as an argument to make. Default is none, to trigger an error
PROLOG=none
PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_ergo_lib_java_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling  .../ErgoAI/ergo_lib/ergo2java:"

.SUFFIXES: $(PROLOGEXT) $(PROLOGHEADEREXT) $(OBJEXT)


# acceptable targets are all (default), system, base, top
all: init $(ALLOBJS) finish
	./CompileChangedFiles.sh $(PROLOG_COMMAND)
	@cat $(SVFILE)


%$(OBJEXT) : %$(PROLOGEXT) $(FLORA_PORTING) $(FLORA_EXTENSIONS) $(FLORA_WRAP)
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
	@echo "file_flush($(STDWARN), _)." >> cmd...


finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


clean:
	/bin/rm -f core *~ .*~ *$(OBJEXT) *.bak .#* cmd* *$(OBJEXT)


.DONE:
	@echo Done
