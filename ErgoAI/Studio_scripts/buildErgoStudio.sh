#!/bin/sh

# This script is supposed to be run as
#
#    ./ErgoEngine/ErgoAI/Studio_scripts/buildErgoStudio.sh
# or
#    ./ErgoEngine/ErgoAI/Studio_scripts/buildErgoStudio.sh  studio-directory
#
# from a directory in which the Ergo souces are in the subdirectory ./ErgoEngine
# and XSB sources are in the subdirectory ./XSB.
# Both 'ErgoEngine' and 'XSB' can be symlinks to the directories where ErgoAI
# and XSB reside physically.

# The studio Git sources (the Git top level) should be in the subdirectory
# called 'Studio_fidji' (can be a symlink). This should have the subdirectory
# interprologForJDK.
# Alternatively, provide the location of the Studio directory for sources
# using the optional argument to buildErgoStudio.sh.
# That directory is called interprologForJDK and contains
# build.xml, externalJars/, src/com/, src/org/, among others.

# How to create a folder satisfying all the above requirements?
# =============================================================
# Certainly not by copying XSB/ErgoAI/Studio folders!
# Instead, create an empty folder, say ErgoAI, and then create soft links
# like this:
#    ln -s  ...\ErgoEngine  ErgoEngine   <--- assuming the Ergo sources are here
#    ln -s  ...\XSB     XSB      <--- assuming the XSB sources are here
#    ln -s  ...\Studio_fidji   Studio_Studio   <--- assuming the Studio sources are here

# IMPORTANT:
#  1. Install Oracle JDK or OpenJDK - not just the JRE!
#  2. Install Ant from your OS's package repository
#  3. JAVA_HOME must point to the JDK, NOT JRE, if it is not already set!


CurDir=`pwd`
XSBBIN=$CurDir/XSB/bin
ErgoDir=$CurDir/ErgoEngine/ErgoAI

if [ "$1" = "" ]; then
    Studio=$CurDir/Studio_fidji/interprologForJDK
else
    Studio=$1
fi

cd $Studio
echo
echo '**** Building Studio'
ant -DXSB_BIN_DIRECTORY=$XSBBIN -DERGODIR=$ErgoDir -f build.xml
echo
echo '**** Building Interprolog'
ant -DXSB_BIN_DIRECTORY=$XSBBIN -DERGODIR=$ErgoDir -f build.xml interprologJar
#ant -DXSB_BIN_DIRECTORY=$XSBBIN -DERGODIR=$ErgoDir -f build.xml ergoCallsJavaJar

echo ""
echo Setting up $CurDir and $ErgoDir ...
#cp ergoStudio.jar $CurDir
cp ergoStudio.jar $ErgoDir/ergo_lib/ergo2java/java/ergoStudio-pure.jar
#cp interprolog.jar $CurDir
cp interprolog.jar $ErgoDir/java
# ergoCallsJava.jar is not being built at present
#cp ergoCallsJava.jar $ErgoDir/ergo_lib/ergo2java/java

cp src/com/declarativa/interprolog/interprolog.P $ErgoDir/ergo_lib/ergo2java

cp src/ergoAIFactory.prefs $CurDir
cp $ErgoDir/Studio_scripts/runErgoAI.exe $CurDir
cp $ErgoDir/Studio_scripts/runErgoAI.sh  $CurDir

echo
echo '**** Building ErgoOWL'
cd $ErgoDir/Tools/jena/ErgoOWL
ant
echo
echo '**** Building ErgoSPARQL'
cd $ErgoDir/Tools/jena/ErgoSPARQL
ant
echo
echo '**** Building combined  ergoStudio.jar'
cd $ErgoDir/ergo_lib/ergo2java/java
ant -DGITDIR=$Studio/..

echo Done
