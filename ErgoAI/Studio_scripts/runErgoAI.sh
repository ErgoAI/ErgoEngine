#! /bin/sh

################################################################################
# Note:  Prior to the first run of this script, run
#        ErgoAI/ergoAI_config.sh 
################################################################################

# If readlink exists - use it
thisscript=`(readlink -f "$0") 2> /dev/null || (readlink "$0") 2> /dev/null || echo "$0"`
thisdir=`dirname "$thisscript"`

if [ ! -d $thisdir/ErgoEngine -o ! -d $thisdir/Studio_fidji ]; then
    echo ""
    echo +++++ This script gets copied into the directory that contains
    echo +++++ both ErgoEngine and Studio_fidji, and it must be run from there
    echo ""
    exit 1
fi

ERGOENGINE=
if  [ -d "$thisdir/ErgoEngine/ErgoAI" ]; then
    ERGOENGINE="ErgoEngine/"
fi

ergo2java_jar="$thisdir/${ERGOENGINE}ErgoAI/ergo_lib/ergo2java/java/ergoStudio.jar"

if [ -f $thisdir/NLP/joda-time.jar -a -f $thisdir/NLP/xom.jar ]; then
    withNLP=yes
fi

if [ "`uname`" = "Darwin" ]; then
    dock_option=-Xdock:name=ErgoAI
    memory=6
else
    # get physical memory; if free -g does not exist, set memory to 4
    memory=`(free -g 2> /dev/null || echo "Mem: 4") |awk '/Mem:/ {print $2}'`
fi

# -Djava.net.preferIPv4Stack=true - needed for ehcache -- unused
#IPv4Option=-Djava.net.preferIPv4Stack=true

# run with NLP only if has enough memory
if [ "$withNLP" = "yes" -a $memory -ge 5 ]; then
    # run with NLP
    java $dock_option -Xss3M -Xmx3G $IPv4Option -jar $ergo2java_jar -basedir $thisdir $*
else
    # run without NLP
    java $dock_option $IPv4Option -jar $ergo2java_jar -basedir $thisdir $*
fi

# to see the log, start as
# runErgoAI.sh -d -printlog
