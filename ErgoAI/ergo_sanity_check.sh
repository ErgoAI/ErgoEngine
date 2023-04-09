#! /bin/sh

# Check installation and precompilation
# Used by makeergo  and  ergoAI_config.sh

# ergo_sanity_check.sh [path-to-XSB-executable] [compiling]

OBJEXT=.xwam
PROLOGEXT=.P

if test "$2" != compiling -a ! -f genincludes/flrtable.flh ; then
    echo ""
    echo "+++++ ErgoAI must first be compiled with the makeergo command"
    echo ""
    exit 1
fi

if test -n "$1" -a "$1" != none ; then
    PROLOG="$1"
elif test -x ../XSB/bin/xsb ; then
    PROLOG=../XSB/bin/xsb
else
    PROLOG=`which xsb`
fi

if [ -z "$PROLOG" ] ; then
    echo ""
    echo "+++++ Usage:"
    if [ -n "$2" ] ; then
	echo "+++++   makeergo all path-to-/XSB/bin/xsb"
    else
	echo "+++++   ergo_lite_config.sh path-to-/XSB/bin/xsb"
    fi
    echo ""
    exit 1
fi

echo ""
echo "+++++ Using Prolog engine: $PROLOG"
echo ""

("$PROLOG" -e "halt." 2>&1) > /dev/null || xsbinstallerror=true
if test -n "$xsbinstallerror" ; then
    echo ""
    echo "+++++ Engine $PROLOG failed to start:"
    echo "+++++    XSB has not been configured properly at that location"
    echo ""
    exit 1
fi

("$PROLOG" -e "[flrdepstest]. halt." 2>&1) > /dev/null || xsbsourceserror=true
if test -n "$xsbsourceserror" ; then
    echo ""
    echo "+++++ XSB sources do not seem to have been installed"
    echo ""
    exit 1
fi

# This causes recompilation of all .flr files by cleaning out
# the .xsb/ergo-* directories.
/bin/rm -f "$HOME"/.xsb/ergo*/*$PROLOGEXT "$HOME"/.xsb/ergo*/*$OBJEXT

# Generate .ergo_buildinfo.P & .ergo_paths
"$PROLOG" -e "[flrconfig]. halt." || ergousageerror=true
if test -n "$ergousageerror" ; then
    echo ""
    echo "+++++ Failed to configure ErgoAI Reasoner"
    echo "+++++ Please report to ergoai-xsb-forum@coherentknowledge.com and include the installation log"
    echo ""
    exit 1
fi

echo ""
echo "+++++ All is well: you can now run ErgoAI Reasoner with the script"
echo "+++++    ./ErgoAI/runergo"
echo ""

default_tabling=flrincludes/.ergo_default_tabling
echo "" > $default_tabling
