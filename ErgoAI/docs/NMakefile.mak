## File:      ErgoAI/docs/NMakefile.mak
##
## Author(s): Michael Kifer
##            Guizhen Yang
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) by
##      The Research Foundation of the State University of New York, 1999-2013.
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
##

CLEAN :
	-@if exist *~ erase *~
	-@if exist *.bak erase *.bak
	-@if exist .#* erase .#*

copy:
	-@(where /q pdflatex && pdflatex ergo-manual) || echo No LaTex found
	-@where /q bibtex && bibtex ergo-manual
	-@where /q pdflatex && pdflatex ergo-manual
	-@where /q pdflatex && pdflatex ergo-manual
	-@where /q pdflatex && pdflatex ergo-packages
	-@where /q bibtex && bibtex ergo-packages
	-@where /q pdflatex && pdflatex ergo-packages
	-@where /q pdflatex && pdflatex ergo-packages
