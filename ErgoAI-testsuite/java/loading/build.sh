#! /bin/sh

FLORAJAVABASE=$HOME/FLORA/flora-git/flora2/java

javac -classpath $FLORAJAVABASE/interprolog.jar:$FLORAJAVABASE/flora2java.jar \
     LoadTest.java 

