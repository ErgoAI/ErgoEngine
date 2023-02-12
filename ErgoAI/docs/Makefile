## File:      ErgoAI/docs/Makefile
##
## Author(s): Michael Kifer
##
## Contact:   flora-users@lists.sourceforge.net
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


all:  ergo-manual.pdf ergo-packages.pdf

ergo-manual.dvi: ergo-manual.tex 
	@echo
	@echo Running LaTeX on ergo-manual.tex
	latex ergo-manual
	@echo
	@echo Making bibliography...
	bibtex ergo-manual
	@echo
	@echo Making index...
	makeindex ergo-manual
	@echo
	@echo Recompiling...
	latex ergo-manual
	latex ergo-manual
	latex ergo-manual

ergo-manual.pdf: ergo-manual.dvi
	@echo
	@echo Making pdf for ergo-manual
	dvipdf ergo-manual

#ergo-packages.dvi: ergo-packages.tex pkg-ergo-persistence.tex pkg-ergo-xml.tex pkg-java-ergo.tex
ergo-packages.dvi: ergo-packages.tex
	@echo
	@echo Running LaTeX on ergo-packages.tex
	latex ergo-packages
	bibtex ergo-packages
	latex ergo-packages
	latex ergo-packages
	makeindex ergo-packages

ergo-packages.pdf: ergo-packages.dvi
	@echo
	@echo Making pdf for ergo-packages
	dvipdf ergo-packages


clean:
	/bin/rm -f  *.bak *~ .*~ .#* *.aux *.toc *.ind *.dvi *.bbl *.blg *.log
	/bin/rm -f  *.out *.idx *.ilg

veryclean: clean
	/bin/rm -f  *.aux *.bbl *.blg *.dvi *.idx *.ilg *.ind *.log \
			*.out ergo*.pdf flora*.pdf *.ps *.toc
