## File:      ErgoAI/ergo_syslib/Makefile
##
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) Coherent Knowledge Systems, LLC, 2013-2018.
##


OBJEXT=.xwam
PROLOGEXT = .P
PROLOGHEADEREXT=.H

PROLOG = none
FLORADIR = ..

ALLOBJS =  \
	  ergo_struct_db$(OBJEXT) \
	  ergo_dbconstraint$(OBJEXT) \
	  ergo_alert$(OBJEXT) \
	  ergo_textit$(OBJEXT) \
	  ergo_fastloader$(OBJEXT) \
	  ergo_random$(OBJEXT) \
	  fidji-cks$(OBJEXT)

FLORA_INCLUDES_DIR = ../flrincludes
DYN_INCLUDES_DIR = ../includes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_ERRORS = $(FLORA_INCLUDES_DIR)/flora_errors.flh
FLORA_EXCEPTIONS = $(FLORA_INCLUDES_DIR)/flora_exceptions.flh
FLRHEADER = $(DYN_INCLUDES_DIR)/flrheader.flh


OPTIONS = [optimize,ti_all,spec_repr]

PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_ergo_syslib_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/ergo_syslib:"

.SUFFIXES: $(PROLOGEXT) $(PROLOGHEADEREXT) $(OBJEXT)

all: init $(ALLOBJS) finish
	./CompileChangedFiles.sh $(PROLOG_COMMAND)
	@cat $(SVFILE)


%$(OBJEXT): %$(PROLOGEXT) $(FLORA_TERMS) $(FLORA_EXCEPTIONS)
	@echo "mc('$*',$(OPTIONS))." >> cmd...

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
	/bin/rm -f core *~ .*~ *$(OBJEXT) *.bak .#* cmd*


.DONE:
	@echo Done
