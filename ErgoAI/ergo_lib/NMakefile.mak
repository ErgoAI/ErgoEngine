## File:      ergo_lib/NMakefile.mak - Make file for NMAKE and ergo_lib
##
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) Coherent Knowledge Systems, 2015 - 2018.
##
##


OBJEXT = .xwam
PROLOGEXT = .P

ALLOBJS = ergo2java$(PROLOGEXT) \
	  ergo2sql$(PROLOGEXT) \
	  ergo2http$(PROLOGEXT)  \
	  ergo2sparql$(PROLOGEXT) \
	  ergo2owl$(PROLOGEXT) \
	  ergo2json$(PROLOGEXT) \
	  ergo_explain$(PROLOGEXT) \
	  ergo_set$(PROLOGEXT) \
	  fidjiUtils$(PROLOGEXT)

OPTIONS = [optimize]

.SUFFIXES: $(PROLOGEXT) .ergo

ALL: $(ALLOBJS)
	cd ergo2java
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
        cd ..\ergo2sparql
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
        cd ..\ergo2owl
	nmake /nologo /f NMakefile.mak PROLOG="$(PROLOG)" PROLOGDIR="$(PROLOGDIR)"
        cd ..


$(ALLOBJS): ..\flrincludes\flora_terms.flh
.ergo$(PROLOGEXT):
	"$(PROLOG)" -e "add_lib_dir(a('..')). add_lib_dir(a('..\\ergo_syslib')). ['..\\flora2']. import '_#flmakesetup'/0 from flora2. '_#flmakesetup'. import flora_compile_system_module/1 from flrutils. flora_compile_system_module(%|fF). halt."


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
	cd ergo2java
	nmake /nologo /f NMakefile.mak clean
	cd ..\ergo2owl
	nmake /nologo /f NMakefile.mak clean
	cd ..\ergo2sparql
	nmake /nologo /f NMakefile.mak clean
        cd ..
