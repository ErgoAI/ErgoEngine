@echo off
REM This script must be in ...\ErgoEngine\ErgoAI\Studio_scripts\runErgoAI.bat
REM Execute it in the directory where ErgoEngine\ErgoAI is installed

REM It is used mostly for debugging
REM Debug mode:  add -debug on cmd line

set DEBUG=

set BASEDIR=%0\..\..\..\..

if [%1] == [-debug] (
    set DEBUG="-d -printlog"
)

set ERGOENGINE=
if exist "%BASEDIR%\ErgoEngine\ErgoAI"  (
    set ERGOENGINE=ErgoEngine\
)

set ERGO2JAVAJAR="%BASEDIR%\%ERGOENGINE%ErgoAI\ergo_lib\ergo2java\java\ergoStudio.jar"

REM java -Dfile.encoding=UTF-8 -jar %ERGO2JAVAJAR% -basedir %BASEDIR% %DEBUG% 2> .ergo_debug.log
java -jar %ERGO2JAVAJAR% -basedir %BASEDIR% %DEBUG% 2> .ergo_debug.log
