# File:      unixVariables.sh
#
# Author(s): Aditi Pandit
#
# Contact:   see  ../CONTACTS.txt
#
# Copyright (C) by
#      The Research Foundation of the State University of New York, 1999-2018.
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



# Change JAVA_BIN if necessary

# Normally doesn't need to be set: autodetected
JAVA_BIN=

# don't touch this
if [ -z "$JAVA_BIN" ]; then
    JAVA_PROG=`which java 2> /dev/null` 

    if [ -z "$JAVA_PROG" -o ! -x "$JAVA_PROG" ]; then
       echo "** Java executable could not be found on your system"
       echo "** Please set the variable JAVA_BIN to the Java binary directory"
    else
	JAVA_BIN=`dirname "$JAVA_PROG"`
	echo "** Using Java executable in $JAVA_BIN"
	$JAVA_BIN/java -version
	echo "** Please make sure that Java has version 1.8 or later"
    fi
fi

