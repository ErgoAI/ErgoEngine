## File:      ErgoAI/ergo_pkgs/Makefile
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
##
## Copyright (C) Coherent Knowledge Systems, 2014-2018.
##
##



OBJEXT=.xwam
PROLOGEXT=.P

FLORADIR=..

ALL = 	evidential_probability${PROLOGEXT} \
	minizinc${PROLOGEXT} \
	e2dsv${PROLOGEXT}

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
SVFILE = '$(FLORADIR)/.__flora_ergo_pkgs_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling .../ErgoAI/ergo_pkgs:"


# suffix rules
.SUFFIXES: $(PROLOGEXT) .ergo

all: clean init recompile
	cd evidential_probability && $(MAKE) PROLOG=$(PROLOG)
	cd e2dsv && $(MAKE) PROLOG=$(PROLOG)

recompile: $(ALL) finish
	@../touch.sh cmd...
	$(PROLOG_COMMAND) < cmd...
	@rm cmd...
	@cat $(SVFILE)


evidential_probability$(PROLOGEXT):
	echo "import ('\\\\compile')/1 from flora2. '\\\\compile'($* >> '\\ep')." >> cmd...

.ergo${PROLOGEXT}:
	echo "import ('\\\\compile')/1 from flora2. '\\\\compile'($*)." >> cmd...

init:
	/bin/rm -f $(SVFILE)
	@echo "add_lib_dir(a('..'))." >> cmd...
	@echo "['../flora2']." >> cmd...
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...
	@echo "'_#flmakesetup'." >> cmd...
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
	cd evidential_probability && make clean
