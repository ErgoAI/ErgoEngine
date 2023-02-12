#! /bin/sh

FLORAJAVABASE=$HOME/FLORA/flora-git/flora2/java

. $FLORAJAVABASE/unixVariables.sh > /dev/null
. $FLORAJAVABASE/flora_settings.sh > /dev/null

java -DPROLOGDIR="${PROLOGDIR}" -DFLORADIR="${FLORADIR}" \
     -classpath .:$FLORAJAVABASE/interprolog.jar:$FLORAJAVABASE/flora2java.jar \
     LoadTest

