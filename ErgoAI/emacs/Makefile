## File:      ErgoAI/emacs/Makefile
##
## Author(s): Michael Kifer
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

all: ergo.elc

ergo.elc: ergo.el
	emacs -batch -f batch-byte-compile ergo.el

clean:
	/bin/rm -f *.bak *~ .*~ .#*

veryclean: clean
	/bin/rm -f *.elc
