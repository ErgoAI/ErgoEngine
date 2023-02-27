@echo off
REM File:      windowsVariables.bat
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

REM Change JAVA_BIN if necessary

if "%JAVA_HOME%" == "" (
   for /d %%i in ("C:\Program Files\Java\jdk*") do set JAVA_HOME=%%i
)

set JAVA_BIN="%JAVA_HOME%\bin"
REM set JAVA_BIN manually below, if the above automatic method fails
REM set JAVA_BIN="C:\Program Files\Java\jdk1.8.0_25\bin"

@echo ** Using Java executable in %JAVA_BIN%
%JAVA_BIN%\java -version
@echo ** Please make sure that Java has version 1.8 or later

