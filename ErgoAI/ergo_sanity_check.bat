@echo off

REM Check installation and precompilation
REM Used by makeergo.bat  and  ergo_customer.iss/ergo_dev.iss

REM ergo_sanity_check.bat [path-to-XSB-executable] [compiling]

@set OBJEXT = .xwam
@set PROLOGEXT = .P

if [%1] == [] goto ergousage

if [%2] == [] (if not exist genincludes\flrtable.flh goto ergonotcompilederror)

@echo.
@set PROLOG=%1
call %PROLOG% -e "halt." > null 2>&1 || goto xsbinstallerror
call %PROLOG% -e "[flrdepstest]. halt." > null 2>&1 || goto xsbsourceserror
del null

REM This for-loop causes recompilation of all .flr/.ergo files by cleaning
REM out the .xsb\ergo-* directories
for /D %%i in (%USERPROFILE%\.xsb\"ergo-*") do if exist "%%i\*%OBJEXT%" (del /Q "%%i\*%OBJEXT%" "%%i\*%PROLOGEXT%"
)

REM Generate .ergo_buildinfo.P & .ergo_paths
call %PROLOG% -e "[flrconfig]. halt." || goto ergoconfigerror

if [%2] == [] goto success

goto end

:success
@echo.
@echo.
@echo +++++ All is well: you can now run ErgoAI Engine using the script
@echo +++++    runergo.bat
@echo.
goto end

:xsbinstallerror
@echo.
@echo.
@echo +++++ Engine %PROLOG% has failed to start:
@echo +++++    XSB has not been configured properly at that location
@echo.
goto end

:xsbsourceserror
@echo.
@echo.
@echo +++++ XSB sources do not seem to have been installed
@echo.
goto end

:ergonotcompilederror
@echo.
@echo.
@echo +++++ ErgoAI must first be compiled with the makeergo.bat command
@echo.
goto end

:ergoconfigerror
@echo.
@echo.
@echo +++++ Failed to configure ErgoAI Engine
@echo +++++ Please report to ergoai-xsb-forum@coherentknowledge.com and include the log
@echo.
goto end

:ergousage
@echo.
@echo.
@echo +++++ Usage:
@echo +++++    ergo_lite_config path-to-\XSB\bin\xsb64.bat
@echo.
goto end

:end
set default_tabling=flrincludes\.ergo_default_tabling
echo. > %default_tabling%

@echo.


REM Local Variables:
REM coding-system-for-write:  iso-2022-7bit-dos
REM End:
