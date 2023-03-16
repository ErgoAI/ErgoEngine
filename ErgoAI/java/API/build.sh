#! /bin/sh

# File:      build.sh
#
# Author(s): Aditi Pandit
#
# Contact:   see  ../CONTACTS.txt
#
# Copyright (C) by
#      The Research Foundation of the State University of New York, 1999-2013.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

. ../unixVariables.sh
. ../flora_settings.sh

rm -f javaAPI/util/net/sf/flora2/API/util/*.class
rm -f javaAPI/src/net/sf/flora2/API/*.class

# Change the location of interprolog, if needed
INTERPROLOG=../interprolog.jar

${JAVA_BIN}/javac -deprecation -Xlint:unchecked -Xlint -classpath ..:${INTERPROLOG} javaAPI/util/net/sf/flora2/API/util/*.java javaAPI/src/net/sf/flora2/API/*.java  

echo
echo The API has been packaged as ../flora2java.jar and ../ergojava.jar
${JAVA_BIN}/jar -cf ../flora2java.jar -C javaAPI/src net/sf/flora2/API -C javaAPI/util net/sf/flora2/API/util
${JAVA_BIN}/jar -cf ../ergojava.jar -C javaAPI/src net/sf/flora2/API -C javaAPI/util net/sf/flora2/API/util
