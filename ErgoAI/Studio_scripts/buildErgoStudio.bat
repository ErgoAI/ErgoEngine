@REM This script is supposed to be run as
@REM
@REM    .\ErgoEngine\ErgoAI\Studio_scripts\buildErgoStudio.sh win-type
@REM or
@REM    .\ErgoEngine\ErgoAI\Studio_scripts\buildErgoStudio.sh wintype ant-dir
@REM
@REM from a directory in where the ErgoAI souces are accessible via
@REM .\ErgoEngine\ErgoAI and XSB sources are in the subdirectory .\XSB.
@REM Both 'ErgoEngine' and 'XSB' can be symlinks to the directories where
@REM ErgoAI and XSB reside physically.

@REM The first argument, win-type is MANDATORY.
@REM It must be 32 (for 32 bit Windows) or 64 (for 64 bit Windows)

@REM The optional ant-dir argument can be used to supply the directory where
@REM Apache Ant is installed. This is needed only if the 'ant' executable
@REM is not found through the Windows %Path% environment variable.
@REM For instance, c:\Users\Me\apache-ant-1.9.13-bin\apache-ant-1.9.13\bin
@REM Note:
@REM    1. No trailing backslash is needed.
@REM    2. If ant-dir has spaces, enclose the argument in ""-quotes as in
@REM       "c:\Users\My Folder\apache-ant-1.9.13-bin\apache-ant-1.9.13\bin"

@REM The studio Git sources (the Git top level) should be in the subdirectory
@REM called 'Studio_fidji' (can be a symlink). This should have the subdirectory
@REM interprologForJDK, which in turn contains build.xml, externalJars\,
@REM src\com\, src\org\, among others.

@REM Example of a call (MK, Win64):
@REM    .\ErgoEngine\ErgoAI\Studio_scripts\buildErgoStudio.bat  64  C:\Users\Michael\Downloads\apache-ant-1.9.13-bin\apache-ant-1.9.13\bin

@REM How to create a folder satisfying all the above requirements?
@REM =============================================================
@REM Certainly not by copying XSB/ErgoAI/Studio folders!
@REM Instead, create an empty folder, say ErgoAI, and then create soft links
@REM like this:
@REM    mklink /j ErgoEngine ...\ErgoEngine   <--- assuming the Ergo sources are here
@REM    mklink /j XSB    ...\XSB     <--- assuming the XSB sources are here
@REM    mklink /j Studio_fidji ...\Studio_fidji   <--- assuming the Studio sources are here
@REM Note: mklink can make links only within the same volume (and only on NTFS).

@REM IMPORTANT:
@REM  1. Download and install Oracle Java SE Development Kit - not just the JRE!
@REM  2. Download and install Ant from https://ant.apache.org/ !!!
@REM  3. JAVA_HOME must point to the JDK, NOT JRE, if it is not already set!
@REM     For instance, set JAVA_HOME=""C:\Program Files\Java\jdk1.8.0_66""
@REM     NOTE: Double "" seem to be necessary for Ant to work.
@REM  4. You might also need to set ANT_HOME appropriatly, if you have not
@REM     provided the ant-dir argument (the optional 2nd argument).

@REM Is it 32 or 64 bit windoze?

@echo off

if [%1] == [32] (
   set WINYTPE=x86
) else set WINTYPE=x64

@set ThisScript=%~dp0
@set ThisScript=%ThisScript:\=/%
@set CurDir=%ThisScript%../../..
@set XSBBIN="%CurDir%/XSB/config/%WINTYPE%-pc-windows/bin"
@set ErgoDir="%CurDir%/ErgoEngine/ErgoAI"
@set Studio="%CurDir%/Studio_fidji/interprologForJDK"

@REM The \ at the end of next line is important - to separate the dir from 'ant'
@if NOT [%2] == []  set AntDirDos=%2\

@set CurDirDos=%~dp0\..\..\..
@set ErgoDirDos=%CurDirDos%\ErgoEngine\ErgoAI
@set StudioDos=%CurDirDos%\Studio_fidji\interprologForJDK

@set RUNNER=runergo.bat

cd "%StudioDos%"


@echo.
@echo **** Building Studio
call %AntDirDos%ant -DXSB_BIN_DIRECTORY=%XSBBIN% -DERGODIR=%ErgoDir% -DERGO_RUNNER=%RUNNER% -f build.xml

@echo.
@echo **** Building Interprolog
call %AntDirDos%ant -DXSB_BIN_DIRECTORY=%XSBBIN% -DERGODIR=%ErgoDir% -DERGO_RUNNER=%RUNNER% -f build.xml interprologJar

REM call %AntDirDos%ant -DXSB_BIN_DIRECTORY=%XSBBIN% -DERGODIR=%ErgoDir% -DERGO_RUNNER=%RUNNER% -f build.xml ergo2javaJar

@REM copy ergoStudio.jar "%CurDirDos%\ergoStudio.jar"
copy ergoStudio.jar "%ErgoDirDos%\ergo_lib\ergo2java\java\ergoStudio-pure.jar"
@REM copy interprolog.jar "%CurDirDos%\interprolog.jar"
copy interprolog.jar "%ErgoDirDos%\java\interprolog.jar"
@REM copy ergo2java.jar "%ErgoDirDos%\ergo_lib\ergo2java\java\ergo2java.jar"

copy "src\com\declarativa\interprolog\interprolog.P" "%ErgoDirDos%\ergo_lib\ergo2java\interprolog.P"

copy "src\ergoAIFactory.prefs" "%CurDirDos%\ergoAIFactory.prefs"
copy "%ErgoDirDos%\Studio_scripts\runErgoAI.exe" "%CurDirDos%\runErgoAI.exe"
copy "%ErgoDirDos%\Studio_scripts\runErgoAI.sh" "%CurDirDos%\runErgoAI.sh"

@echo.
echo **** Building ErgoOWL
cd  "%ErgoDirDos%\Tools\jena\ErgoOWL"
call %AntDirDos%ant
@echo.
echo **** Building ErgoSPARQL
cd  "%ErgoDirDos%\Tools\jena\ErgoSPARQL"
call %AntDirDos%ant
@echo.
echo **** Building combined  ergoStudio.jar
cd  "%ErgoDirDos%\ergo_lib\ergo2java\java"
call %AntDirDos%ant -DDGITDIR=%StudioDos%\..


cd "%CurDir%"
