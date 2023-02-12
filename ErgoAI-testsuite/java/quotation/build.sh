#!/bin/sh

FLORAJAVABASE=$HOME/FLORA/flora-git/flora2/java

. $FLORAJAVABASE/unixVariables.sh
. $FLORAJAVABASE/flora_settings.sh

CP=.:$FLORAJAVABASE/interprolog.jar:$FLORAJAVABASE/flora2java.jar
javac -Xlint:deprecation -cp $CP src/iptest/InterPrologTest.java
