#! /bin/sh

FLORAJAVABASE=$HOME/FLORA/flora-git/flora2/java

javac -classpath $FLORAJAVABASE/interprolog.jar:$FLORAJAVABASE/flora2java.jar:opencsv-4.0.jar:commons-lang3-3.6.jar QuoteTest.java
