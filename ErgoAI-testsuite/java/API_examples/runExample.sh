#! /bin/sh
# File:      runExample.sh
#
# Author(s): Aditi Pandit
#
# Contact:   flora-users@lists.sourceforge.net
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

FLORAJAVABASE=$HOME/FLORA/flora-git/flora2/java

. $FLORAJAVABASE/unixVariables.sh > /dev/null
. $FLORAJAVABASE/flora_settings.sh > /dev/null

. ${1}/floraVariables.sh

INTERPROLOG=$FLORAJAVABASE/interprolog.jar

${JAVA_BIN}/java -DPROLOGDIR="${PROLOGDIR}" \
           -DINPUT_FILE="${INPUT_FILE}" -DFLORADIR="${FLORADIR}" \
           -Djava.library.path="${PROLOGDIR}"  \
           -classpath ${CLASSPATH}:$FLORAJAVABASE/flora2java.jar:${INTERPROLOG}:fooExample:flogicbasicsExample ${1}  2> /dev/null
