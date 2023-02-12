## File:      ErgoAI/Makefile
##
## Author(s): Michael Kifer
##            Guizhen Yang
##
## Contact:   flora-users@lists.sourceforge.net
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 1999-2018;
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

FLORADIR = .

ALLOBJS = flrarguments${OBJEXT} \
	  flrcomposer${OBJEXT} \
	  flrcompiler${OBJEXT} \
	  flrcoder${OBJEXT} \
	  flrdependency${OBJEXT} \
	  flrlexer${OBJEXT} \
	  flrlibman${OBJEXT} \
	  flrnodefp${OBJEXT} \
	  flrnowsp${OBJEXT} \
	  flroperator${OBJEXT} \
          flrparser${OBJEXT} \
	  flrporting${OBJEXT} \
	  flrpretifydump${OBJEXT} \
	  flrprint${OBJEXT} \
	  flrprolog${OBJEXT} \
	  flrregistry${OBJEXT} \
	  flrshell${OBJEXT} \
	  flrshell_loop_handler${OBJEXT} \
	  flrundefined${OBJEXT} \
	  flrundefhook${OBJEXT} \
	  flrutils${OBJEXT} \
	  flrversion${OBJEXT} \
	  flrwraparound${OBJEXT} \
	  flrsynonym${OBJEXT} \
	  flrsimpleprimitive${OBJEXT} \
	  flrnosymbolcontext${OBJEXT} \
	  flrnowarn${OBJEXT} \
	  flrwrapper${OBJEXT}


FLORA_CHAR_DEFS = $(FLORADIR)/flrincludes/flora_characters.flh

FLORA_INCLUDES_DIR = flrincludes

FLORA_TERMS = $(FLORA_INCLUDES_DIR)/flora_terms.flh
FLORA_TABLING_METHODS = $(FLORA_INCLUDES_DIR)/flora_tabling_methods.flh
FLORA_DEFAULT_TABLING = $(FLORA_INCLUDES_DIR)/.ergo_default_tabling
FLORA_ERRORS = $(FLORA_INCLUDES_DIR)/flora_errors.flh
FLORA_EXCEPTIONS = $(FLORA_INCLUDES_DIR)/flora_exceptions.flh
FLORA_PORTING = $(FLORA_INCLUDES_DIR)/flora_porting.flh
FLORA_EXTENSIONS = $(FLORA_INCLUDES_DIR)/flora_extensions.flh
FLORA_WRAP = $(FLORA_INCLUDES_DIR)/flora_wrap.flh

OPTIONS = [optimize,ti_all,spec_repr]

# PROLOG is passed as an argument to make. Default is none, to trigger an error
PROLOG=none
PROLOG_COMMAND = "$(PROLOG)" -m 60000 -c 10000 -e 'segfault_handler(warn).'

# Make sure warnings are saved in a file.
SVFILE = '$(FLORADIR)/.__flora_top_warn.tmp'
STDWARN = 3
STDERR = 2
HDRMSG = "While compiling  .../ErgoAI:"

.SUFFIXES: $(PROLOGEXT) $(PROLOGHEADEREXT) $(OBJEXT)


# acceptable targets are:
#    	     all (default), system, base, top, nodocs (also excludes demos)
all: version core_internal libraries packages alldemos emacsmode manuals showerrors

nodocs: core_internal libraries packages emacsmode showerrors

system: core libraries showerrors

base: core showerrors

showerrors:
	@cat $(FLORADIR)/.__flora*.tmp
	@rm $(FLORADIR)/.__flora*.tmp

version:
	@(update_version.sh 2> /dev/null && echo "\n*** ErgoAI version has been updated.\n") || echo "\n*** WARNING: Not a Git-checked-out copy of ErgoAI, or Perl is not installed.\n    Not updating ErgoAI version.\n"

top: init $(ALLOBJS) finish
	./CompileChangedFiles.sh $(PROLOG_COMMAND)
	cd datatypes && $(MAKE) PROLOG=$(PROLOG)
	@cat $(SVFILE)

core_internal: top
	cd closure && $(MAKE) PROLOG=$(PROLOG)
	cd includes && $(MAKE) PROLOG=$(PROLOG)
	cd genincludes && $(MAKE) PROLOG=$(PROLOG)
	cd syslib && $(MAKE) PROLOG=$(PROLOG)
	cd debugger && $(MAKE) PROLOG=$(PROLOG)
	@-(test -d ergo_syslib && cd ergo_syslib && $(MAKE) PROLOG=$(PROLOG)) || echo ""
	@echo "Making ergoact and ergoflt"
	@-(test -d cc/ergoact && cd cc/ergoact && $(MAKE) PROLOG=$(PROLOG)) || echo ""
	@-(test -d cc/ergoflt && cd cc/ergoflt && $(MAKE) PROLOG=$(PROLOG)) || echo ""

core: top 
	rm -f AT/prolog/*.xwam
	rm -f pkgs/prolog/*.xwam
	cd closure && $(MAKE) PROLOG=$(PROLOG)
	cd includes && $(MAKE) PROLOG=$(PROLOG)
	cd genincludes && $(MAKE) PROLOG=$(PROLOG)
	cd syslib && $(MAKE) PROLOG=$(PROLOG)
	cd debugger && $(MAKE) PROLOG=$(PROLOG)
	@-(test -d ergo_syslib && cd ergo_syslib && $(MAKE) PROLOG=$(PROLOG)) || echo ""
	@-(test -d cc/ergoact && cd cc/ergoact && $(MAKE) PROLOG=$(PROLOG)) || echo ""
	@-(test -d cc/ergoflt && cd cc/ergoflt && $(MAKE) PROLOG=$(PROLOG)) || echo ""

libraries:
	# startup optimization for Linux
	echo '\halt.' | $(FLORADIR)/runflora
	cd lib && $(MAKE) PROLOG=$(PROLOG)
	cd AT && $(MAKE) PROLOG=$(PROLOG)
	@-(cd ergo_lib && $(MAKE) PROLOG=$(PROLOG))

packages:
	cd pkgs && $(MAKE) PROLOG=$(PROLOG)
	@-(cd ergo_pkgs && $(MAKE) PROLOG=$(PROLOG))

alldemos:
	cd ./demos && $(MAKE) PROLOG=$(PROLOG)

emacsmode:
	-(cd emacs && $(MAKE))

manuals:
	-test -f docs/Makefile && cd docs && $(MAKE) || echo "" > /dev/null


${FLORA_TABLING_METHODS} : ${FLORA_DEFAULT_TABLING}
	./touch.sh  $(FLORA_TABLING_METHODS)


%$(OBJEXT) : %$(PROLOGEXT) $(FLORA_TERMS) $(FLORA_PORTING) $(FLORA_EXTENSIONS) $(FLORA_WRAP)
	@echo 'mc($*,$(OPTIONS)).' >> cmd...

%$(PROLOGEXT) : %$(PROLOGHEADEREXT)
	./touch.sh $@

flrversion${OBJEXT}: flrversion${PROLOGEXT} version.flh
	@echo 'mc(flrversion,$(OPTIONS)).' >> cmd...

flrporting${OBJEXT}: flrporting${PROLOGEXT} version.flh $(FLORA_TERMS) $(FLORA_EXCEPTIONS)
	@echo 'mc(flrporting,$(OPTIONS)).' >> cmd...

flrlexer${OBJEXT} : flrlexer${PROLOGEXT} flrlexer.H $(FLORA_CHAR_DEFS) $(FLORA_TERMS) $(FLORA_ERRORS) $(FLORA_PORTING)
	@echo 'mc(flrlexer,$(OPTIONS)).' >> cmd...


flrcomposer${OBJEXT} : flrcomposer${PROLOGEXT} flrcomposer.H $(FLORA_CHAR_DEFS) $(FLORA_TERMS) $(FLORA_ERRORS) $(FLORA_PORTING)
	@echo 'mc(flrcomposer,$(OPTIONS)).' >> cmd...


flrparser${OBJEXT} : flrparser${PROLOGEXT} flrparser.H $(FLORA_TERMS) $(FLORA_ERRORS) $(FLORA_PORTING)
	@echo 'mc(flrparser,$(OPTIONS)).' >> cmd...


flrcompiler${OBJEXT} : flrcompiler${PROLOGEXT} flrcompiler.H $(FLORA_TERMS) $(FLORA_ERRORS) $(FLORA_PORTING)
	@echo 'mc(flrcompiler,$(OPTIONS)).' >> cmd...


flrcoder${OBJEXT} : flrcoder${PROLOGEXT} flrcoder.H $(FLORA_CHAR_DEFS) $(FLORA_TERMS) $(FLORA_ERRORS) $(FLORA_PORTING)
	@echo 'mc(flrcoder,$(OPTIONS)).' >> cmd...


flrutils${OBJEXT} : flrutils${PROLOGEXT} flrutils.H $(FLORA_CHAR_DEFS) $(FLORA_TERMS) ${FLORA_TABLING_METHODS}  $(FLORA_PORTING) $(FLORA_EXCEPTIONS)
	@echo 'mc(flrutils,$(OPTIONS)).' >> cmd...



init:
	@/bin/rm -f $(FLORADIR)/.__*flora*.tmp
	@echo "write($(STDWARN),'-------------- Warnings/Errors --------------------'), nl($(STDWARN))." > cmd...
	@echo "write($(STDWARN),'$(HDRMSG)'), nl($(STDWARN))." >> cmd...
	@echo "file_flush($(STDWARN),_)." >> cmd...
	@echo "['./flora2']." >> cmd...hdr
	@echo "retractall(xpp_options(_))." >> cmd...hdr
	@echo "import '_#flmakesetup'/0 from flora2." >> cmd...hdr
	@echo "file_reopen($(SVFILE),a,$(STDWARN),_)." >> cmd...hdr
	@echo "file_clone($(STDWARN),$(STDERR),_)." >> cmd...hdr
	@echo "'_#flmakesetup'." >> cmd...hdr


finish:
	@echo "write($(STDWARN),'-------------------- End --------------------------'), nl($(STDWARN))." >> cmd...


clean: cleantemp
	/bin/rm -f core *~ .*~ *${OBJEXT} *.O *.bak ../flora2${OBJEXT} .#*
	/bin/rm -f .*.tmp .ergo_path* .prolog_path*
	/bin/rm -f .flora_aux_files/*
	/bin/rm -f .ergo_aux_files/*
	cd closure && $(MAKE) clean
	cd datatypes && $(MAKE) clean
	-test -f docs/Makefile && cd docs && $(MAKE) clean || echo "" > /dev/null
	cd genincludes && $(MAKE) clean
	cd includes && $(MAKE) clean
	cd syslib && $(MAKE) clean
	-test -d ergo_syslib && cd ergo_syslib && $(MAKE) clean
	cd lib && $(MAKE) clean
	cd AT && $(MAKE) clean
	cd debugger && $(MAKE) clean
	cd cc && $(MAKE) clean
	-test -d cc/ergoact && cd cc/ergoact && $(MAKE) clean
	-test -d cc/ergoflt && cd cc/ergoflt && $(MAKE) clean
	-test -d ergo_pkgs && cd ergo_pkgs && $(MAKE) clean
	-test -d ergo_lib && cd ergo_lib && $(MAKE) clean
	cd pkgs && $(MAKE) clean
	cd demos && $(MAKE) clean
	/bin/rm -f headerinc/*~ headerinc/.*~
	/bin/rm -f syslibinc/*~ syslibinc/.*~
	/bin/rm -f libinc/*~ libinc/.*~
	/bin/rm -f includes/*~ includes/.*~
	/bin/rm -f flrincludes/*~ flrincludes/.*~

cleantemp:
	/bin/rm -f "$(HOME)"/.xsb/flora*/*${PROLOGEXT} "$(HOME)"/.xsb/flora*/*${OBJEXT}

veryclean: clean
	-test -f docs/Makefile && cd docs && $(MAKE)  veryclean || echo "" > /dev/null
	cd emacs && $(MAKE) veryclean


.DONE:
	@echo Done
