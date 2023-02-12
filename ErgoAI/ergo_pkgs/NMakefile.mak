## File:      ergo_pkgs/NMakefile.mak - Make file for NMAKE and ergo_pkgs
##
## Author(s): Michael Kifer
##
## Contact:   michael.kifer@coherentknowledge.com
##
## Copyright (C) Coherent Knowledge Systems, 2015 - 2018.
##
##


OBJEXT = .xwam
PROLOGEXT = .P

ALLOBJS = evidential_probability$(PROLOGEXT) \
	  minizinc$(PROLOGEXT) \
	  e2dsv$(PROLOGEXT)

OPTIONS = [optimize]

.SUFFIXES: $(PROLOGEXT) .ergo

ALL: $(ALLOBJS)
	cd evidential_probability
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
        cd ..
	cd e2dsv
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
        cd ..


$(ALLOBJS): ..\flrincludes\flora_terms.flh
.ergo$(PROLOGEXT):
	"$(PROLOG)" -e "add_lib_dir(a('..')). ['..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup'. import ('\\compile')/1 from flora2. '\\compile'(%|fF). halt."


CLEAN:
	-@if exist *$(PROLOGEXT) erase *$(PROLOGEXT)
	-@if exist *$(OBJEXT) erase *$(OBJEXT)
	-@if exist *.flh erase *.fdb
	-@if exist *.fld erase *.fld
	-@if exist *.flt erase *.flt
	-@if exist *.fls erase *.fls
	-@if exist *.fls2 erase *.fls2
	-@if exist *.flm erase *.flm
	-@if exist .flora_aux_files\*.xwam del /q .flora_aux_files
	-@if exist .ergo_aux_files\*.xwam del  /q .ergo_aux_files
	-@if exist *~ erase *~
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*
	cd evidential_probability
	nmake /nologo /f NMakefile.mak clean
        cd ..
