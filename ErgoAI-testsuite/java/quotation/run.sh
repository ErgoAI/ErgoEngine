#!/bin/sh

FLORAJAVABASE=$HOME/FLORA/flora-git/flora2/java

. $FLORAJAVABASE/unixVariables.sh > /dev/null
. $FLORAJAVABASE/flora_settings.sh > /dev/null

CP=$FLORAJAVABASE/interprolog.jar:$FLORAJAVABASE/flora2java.jar
java  -cp $CP:src iptest.InterPrologTest $PROLOGDIR/xsb $FLORADIR 
