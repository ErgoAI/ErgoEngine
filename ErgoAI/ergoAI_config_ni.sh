#!/bin/sh 

# Calls ergo_config.sh with an argument, making it a non-interactive install

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


# ergo_dev.sh may pass -devel as 1st argument, so pass $1 on
# both ergo_dev & ergo_config.sh pass -v version.
# niniteractive param seems unused
$thisdir/ergoAI_config.sh $1 $2 $3 noninteractive
