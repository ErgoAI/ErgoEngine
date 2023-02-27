@echo off
REM File:      buildExample.bat
REM
REM Author(s): Aditi Pandit
REM
REM Contact:   see  ../CONTACTS.txt
REM
REM Copyright (C) by
REM      The Research Foundation of the State University of New York, 1999-2018.
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

CALL ..\..\windowsVariables.bat
CALL ..\..\flora_settings.bat

CALL %1%\floraVariables.bat


CALL ..\..\..\runflora.bat -e "\load('%INPUT_FILE%'>>example),\load(javaAPI),write('%FLORA_CLASS%',example,'%JAVA_FILE%'),\halt."

echo Compiling files
%JAVA_BIN%\javac -classpath ..\..\flora2java.jar;..\..\interprolog.jar %1%\*.java

REM Local Variables:
REM coding-system-for-write:  iso-2022-7bit-dos
REM End:
