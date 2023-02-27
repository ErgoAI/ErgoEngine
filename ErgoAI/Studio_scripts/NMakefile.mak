## File:      ergosuite/NMakefile.mak -- Make file for Microsoft NMAKE
##
## Author(s): Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) by  Coherent Knowledge Systems, 2014
##

EXEEXT = .exe
OBJEXT = .obj
SOURCEEXT = .c

ALLOBJS = runErgoAI$(EXEEXT)

.SUFFIXES: $(SOURCEEXT) $(OBJEXT) $(EXEEXT)

## cc is handled specially, by makeflora
ALL::  $(ALLOBJS)
       copy runErgoAI.exe ..\..\..\runErgoAI.exe"

CLEAN :
	-@if exist *~ erase *~
	-@if exist *$(OBJEXT) erase *$(OBJEXT)
	-@if exist *$(EXEEXT) erase *$(EXEEXT)
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*


$(SOURCEEXT)$(EXEEXT):
	cl.exe %|F shlwapi.lib


.SILENT:
