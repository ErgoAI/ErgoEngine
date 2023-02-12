# Make file for prolog2hilog.dll


CPP = cl.exe
LINKER = link.exe

OUTDIR     = windows64
ARCHDIR    = $(PROLOGDIR)\config\x64-pc-windows
ARCHBINDIR = $(ARCHDIR)\bin
ARCHOBJDIR = $(ARCHDIR)\saved.o

## Maybe create just one DLL out of these two?
ALL : "$(OUTDIR)\prolog2hilog.dll"  "$(OUTDIR)\flora_ground.dll"

CLEAN :
	-@if exist *.obj erase *.obj
	-@if exist *.dll erase *.dll
	-@if exist *.exp erase *.exp
	-@if exist *~ erase *~
	-@if exist .#* erase .#*
	-@if exist *.bak erase *.bak


CPP_PROJ = /nologo /MT /W3 /EHsc /O2 /I "$(ARCHDIR)" \
	 /I "$(PROLOGDIR)\emu" /I "$(PROLOGDIR)\prolog_includes" \
	 /D "WIN64" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" \
	 /Fo"$(ARCHOBJDIR)\\" /Fd"$(ARCHOBJDIR)\\" /c 
	

"$(ARCHOBJDIR)\prolog2hilog.obj" :: prolog2hilog.c  cc_config.P
	$(CPP) $(CPP_PROJ) prolog2hilog.c

"$(ARCHOBJDIR)\flora_ground.obj" :: flora_ground.c  cc_config.P
	$(CPP) $(CPP_PROJ) flora_ground.c

LINK_FLAGS_P2H = xsb.lib \
	 /nologo /dll \
	 /machine:x64 /out:"$(OUTDIR)\prolog2hilog.dll" \
	 /libpath:"$(ARCHBINDIR)" 

LINK_FLAGS_GRND = xsb.lib \
	 /nologo /dll \
	 /machine:x64 /out:"$(OUTDIR)\flora_ground.dll" \
	 /libpath:"$(ARCHBINDIR)"

LINK_OBJS_P2H  =  "$(ARCHOBJDIR)\prolog2hilog.obj"
LINK_OBJS_GRND =  "$(ARCHOBJDIR)\flora_ground.obj"

"$(OUTDIR)\prolog2hilog.dll" : $(LINK_OBJS_P2H)
    $(LINKER) @<<
  $(LINK_FLAGS_P2H) $(LINK_OBJS_P2H)
<<

"$(OUTDIR)\flora_ground.dll" : $(LINK_OBJS_GRND)
    $(LINKER) @<<
  $(LINK_FLAGS_GRND) $(LINK_OBJS_GRND)
<<
