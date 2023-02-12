@echo OFF

@set flrshell=ergo_shell

REM     set JAVA_EXECUTABLE manually, if this automatic method fails
REM for /f "usebackq tokens=*" %%i in (`where java.exe`) do set JAVA_EXECUTABLE=%%i
REM     @echo ** Using Java executable in %JAVA_EXECUTABLE%
REM     @echo ** Please make sure that Java has version 1.8 or later

@set thisdir=%0\..\
@call %thisdir%.ergo_paths.bat

set STARTUPOPTIONS=
set PROFILEFLAG=
set PROFILING=
if [%1] == [--devel] (
    @set PROFILEFLAG="-p"
    @set PROFILING="xsb_profiling:profile_unindexed_calls(on),"
    shift
) else set STARTUPOPTIONS=--noprompt --quietload --nofeedback --nobanner

REM @set PROLOGOPTIONS="-m 2000000 -c 50000"
REM @set PROLOGOPTIONS="-p -m 2000000 -c 50000"

@%PROLOG% %STARTUPOPTIONS% %PROLOGOPTIONS% %PROFILEFLAG% -e "%PROFILING% add_lib_dir(a(%FLORADIR%)). [flora2]. %flrshell%." %1 %2 %3 %4 %5 %6 %7

REM If invoked from a shortcut (desktop icon), it will have a /k parameter
REM in %cmdcmdline%. Then DOUBLECLICKED will be defined and 'exit no' will
REM exit the cmd.exe window.
for %%x in (%cmdcmdline%) do if /i "%%~x"=="/k" @set DOUBLECLICKED=1
if defined DOUBLECLICKED @exit no

