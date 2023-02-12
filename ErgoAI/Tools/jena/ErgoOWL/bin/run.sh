#! /bin/sh

# If readlink exists - use it
thisscript=`(readlink -f "$0") 2> /dev/null || (readlink "$0") 2> /dev/null || echo "$0"`
thisdir=`dirname "$thisscript"`

# a hack for BSD and Macs
case "$thisdir" in
    /*)
	;;
    *)
	if [ "$thisscript" != "$0" ] ; then
	    thisscript=`dirname "$0"`/$thisscript
	    thisdir=`dirname "$thisscript"`
	fi
	;;
esac

jenadir=$thisdir/../..
ergodir=$thisdir/../../../..

#java -cp ".:$jenadir/lib/*:$ergodir/ergo_lib/ergo2owl/java/ergoOWL.jar" ErgoOWL_GUI $HOME
java -cp ".:$jenadir/JenaAllInOne.jar:$ergodir/ergo_lib/ergo2java/java/ergoStudio.jar:$ergodir/ergo_lib/ergo2owl/java/ergoOWL.jar" com.coherentknowledge.ergo.owl.gui.ErgoOWL_GUI $HOME
