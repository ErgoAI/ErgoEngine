
# $1 is expected to be -DDEBUG, if debugging output is needed

FLORAJAVABASE=$HOME/FLORA/flora-git/flora2/java

. $FLORAJAVABASE/unixVariables.sh > /dev/null
. $FLORAJAVABASE/flora_settings.sh > /dev/null

java $1 -DPROLOGDIR="$PROLOGDIR" -DFLORADIR="$FLORADIR" \
     -classpath .:$FLORAJAVABASE/interprolog.jar:$FLORAJAVABASE/flora2java.jar \
     TestQuery 2> /dev/null
