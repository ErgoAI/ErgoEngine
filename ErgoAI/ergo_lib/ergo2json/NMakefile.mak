## File:      ergo_lib/ergo2json/NMakefile - Make file for Microsoft NMAKE
##
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) Coherent Knowledge Systems, Inc., 2016
##
##


OBJEXT = .xwam
PROLOGEXT = .P

ALLOBJS =  ergo_json$(OBJEXT)

OPTIONS = [optimize,ti_all,spec_repr]

.SUFFIXES: $(PROLOGEXT) $(OBJEXT)

ALL:: $(ALLOBJS)

CLEAN :
	-@if exist *~ erase *~
	-@if exist *$(OBJEXT) erase *$(OBJEXT)
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*


## %|fF as a file spec means: %|...F - file parts selection syntax. f- take
##                                     just the base name
$(PROLOGEXT)$(OBJEXT):
$(ALLOBJS): ..\..\flrincludes\flora_terms.flh
$(ALLOBJS): ..\..\flrincludes\flora_exceptions.flh
	"$(PROLOG)" -e "add_lib_dir(a('..\\..')). ['..\\..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup',mc(%|fF,$(OPTIONS)). halt."


