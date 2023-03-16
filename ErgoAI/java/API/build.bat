@echo off
REM File:      build.bat
REM
REM Author(s): Aditi Pandit
REM
REM Contact:   see  ../CONTACTS.txt
REM
REM Copyright (C) by
REM      The Research Foundation of the State University of New York, 1999-2013.
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM      http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.
REM
REM



echo ==============================
echo Building javaInterfaceCode

CALL ..\windowsVariables.bat
CALL ..\flora_settings.bat

REM clean up *.class
for /d %%i in ("javaAPI\util\net\sf\flora2\API\util\*.class") do del %%i
for /d %%i in ("javaAPI\src\net\sf\flora2\API\*.class") do del %%i

echo ------------------------------
echo Compiling files ...
echo ------------------------------

REM Change the location of interprolog, if needed
set INTERPROLOG="..\interprolog.jar"

%JAVA_BIN%\javac -Xlint  -classpath ..;%INTERPROLOG% javaAPI\util\net\sf\flora2\API\util\*.java javaAPI\src\net\sf\flora2\API\*.java 

echo.
echo *** If you saw the warning "The system cannot find the path specified",
echo *** change the variable JAVA_BIN in ..\java\windowsVariables.bat
echo *** appropriately for your system.

echo.
echo The API has been packaged as ..\flora2java.jar and ..\ergojava.jar
%JAVA_BIN%\jar -cf ..\flora2java.jar javaAPI\src\ javaAPI\util\
%JAVA_BIN%\jar -cf ..\ergojava.jar javaAPI\src\ javaAPI\util\


REM Local Variables:
REM coding-system-for-write:  iso-2022-7bit-dos
REM End:
