#!/bin/sh

# !!! You need to manually set the two variables below: ERGOROOT and XSBARCHDIR

# To find the right ERGOROOT directory for your installation,
# start Ergo and type
#     system{installdir=?Dir}.
# For instance:
#ERGOROOT="$HOME/ERGO/ergosuite/Ergo"
# (omit the leading #)

# To find the right XSBARCHDIR directory for your installation,
# start Ergo and type
#     system{archdir=?Dir}.
# For instance:
#XSBARCHDIR="$HOME/XSB/XSB/config/i386-apple-darwin17.3.0"
# or
#XSBARCHDIR="$HOME/XSB/XSB/config/x86_64-unknown-linux-gnu"
# (omit the leading #)
echo
echo ERGOROOT = $ERGOROOT
echo XSBARCHDIR = $XSBARCHDIR
if test -z "$ERGOROOT" -o -z "$XSBARCHDIR" ; then
    echo "*** You must set ERGOROOT and XSBARCHDIR. See the comments in runpyergo.sh"
    echo
    exit 1
fi

# This trick makes this script work in any directory
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

cd $thisdir

# NOTE: PYERGO requires the package 'six'. To install it, run
#           pip install six
#       in a terminal window.
#       pip should have been installed together with pythom.

# In many Linux distributions, 'python' means Python 2.7.
# Python 3.* is usually called 'python3'.
# PYERGO works with either.
python3 $ERGOROOT/python/pyergo_example.py $XSBARCHDIR $ERGOROOT



