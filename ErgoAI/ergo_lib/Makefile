## File:      ErgoAI/ergo_lib/Makefile
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) Coherent Knowledge Systems, 2015 - 2018.
##
##



OBJEXT=.xwam
PROLOGEXT=.P

FLORADIR=..

ALL =   ergo2java${PROLOGEXT} \
	ergo2sql${PROLOGEXT}  \
	ergo2http${PROLOGEXT}  \
	ergo2sparql${PROLOGEXT} \
	ergo2owl${PROLOGEXT} \
	ergo2json${PROLOGEXT} \
	ergo_explain${PROLOGEXT} \
	ergo_set${PROLOGEXT} \
	fidjiUtils${PROLOGEXT}

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
SVFILE = '$(FLORADIR)/.__flora_ergo_lib_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/ergo_lib:"


# suffix rules
.SUFFIXES: $(PROLOGEXT) .ergo

all: clean compile_subdirs init recompile

recompile: $(ALL) finish
	@../touch.sh cmd...
	$(PROLOG_COMMAND) < cmd...
	@rm cmd...
	@cat $(SVFILE)

compile_subdirs:
	cd ergo2java && $(MAKE) PROLOG=$(PROLOG)
	cd ergo2sparql && $(MAKE) PROLOG=$(PROLOG)
	cd ergo2owl && $(MAKE) PROLOG=$(PROLOG)
	cd ergo2json && $(MAKE) PROLOG=$(PROLOG)
	cd ergo_explain && $(MAKE) PROLOG=$(PROLOG)

.ergo${PROLOGEXT}:
	echo 'flora_load_system_module_by_file($*).' >> cmd...

init:
	/bin/rm -f $(SVFILE)
	@echo "add_lib_dir(a('..'))." >> cmd...
	@echo "add_lib_dir(a('../ergo_syslib'))." >> cmd...
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
	/bin/rm -f core *$(PROLOGEXT) *${OBJEXT} 
	/bin/rm -rf .flora_aux_files
	/bin/rm -rf .ergo_aux_files
	cd ergo2java && make clean
	cd ergo2sparql && make clean
	cd ergo2owl && make clean
