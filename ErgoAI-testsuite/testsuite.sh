#! /bin/sh

## File:      testsuite.sh
## Author(s): XSB team
## Contact:   See ErgoAI/CONTACTS.tex
## 
## Copyright (C) by
##      The Research Foundation of the State University of New York, 2000-2023
##
## 
##

#==================================================================
# This is supposed to automate the testsuite by checking the
# log for possible errors.
#==================================================================

#Usage: testsuite.sh [-opts opts] [-tag tag] [-exclude exclude_dirs] \
#    	     	     [-add add_dirs] [-only only_dirs [-mswin] path
# where: opts         -- options to pass to PROLOG executable
#        tag          -- the configuration tag to use
#        exclude_dirs -- the list of tests (in quotes) to NOT run
#        add_dirs     -- list of test directories to add 
#    	     	     	 to the normally tested directories
#        only_dirs    -- list of test directories.
#                        Use these instead of the default
#    	     	     	 For convenience, -add/exclude options are honored
#        path         -- full path name of the Ergo installation directory
#                        or of an alternative runergo script.
#                        This script must be in the Ergo installation folder.
#    The -mswin option says to run tests under MS Windows
#    If tag specified, this is the tag to use to 
#    locate the PROLOG executable (e.g., dbg, chat, etc.). 
#    It is usually specified using the --config-tag option of 'configure'
#    The PROLOG executable is either in
#    	 $1/config/architecture/bin/xsb
#    or in
#    	 $1/config/architecture-tag/bin/xsb
#    depending on whether the configuration tag was given on command line.

echo ==========================================================================


if test -n "$USER"; then
   USER=`whoami`
   export USER
fi


while test 1=1
do
    case "$1" in
     *-opt*)
	    shift
	    options=$1
	    shift
	    ;;

     *-exclud*)
	    shift
	    excluded_tests=$1
	    shift
	    ;;

     *-add*)
	    shift
	    added_tests=$1
	    shift
	    ;;

     *-only*)
	    shift
	    only_tests=$1
	    shift
	    ;;

     *-tag*)
	    shift
	    config_tag=$1
	    shift
	    ;;

      *-mswin*)
	    shift
	    windows=true
	    echo "Running tests under Windows"
	    ;;

      *)
	    break
	    ;;
    esac
done

ERGODIR=$1
testdir=`pwd`
if test -z "$ERGODIR" ; then
   echo "***Ergo installation directory hasn't been specified."
   ERGODIR=$testdir/../ErgoAI
   echo "***Assuming $ERGODIR"
fi
if test ! -d "$ERGODIR" ; then
    if test ! -x "$ERGODIR" ; then
       echo "$ERGODIR: Ergo installation directory doesn't exist or has wrong permissions"
       exit 1
    else
	ERGODIR=`dirname $1`
	runergo_script=`basename $1`
    fi
fi

# install dir argument
if test $# -gt 1; then
    echo "Usage: testsuite.sh [-opts opts] [-tag tag] [-exclude \"excl_dirs\"] [-add \"add_dirs\"] [-mswin] path"
    echo "where: opts      -- options to pass to PROLOG"
    echo "       tag       -- the configuration tag to use"
    echo "       excl_dirs -- the dirs in which not to run tests"
    echo "       add_dirs  -- list of dirs to add to the normally tested dirs"
    echo "       path      -- full path name of the Ergo installation folder"
    echo "                    or of an alternative runergo script. The script"
    echo "                    must be in the Ergo installation folder."
    exit
fi


if test -n "$config_tag" ; then
    config_tag="-$config_tag"
fi


if test -n "$windows"; then
    runergo_script=runergo.bat
    scripttest_flag=-e
else
    if test -z "$runergo_script"; then
	# if runergo_script is not set, set it
	runergo_script=runergo
    fi
    scripttest_flag=-x
fi


GREP="grep -i"
MSG_FILE=/tmp/ergoai_test_msg.$USER
LOG_FILE=/tmp/ergoai_test_log.$USER
RES_FILE=/tmp/ergoai_test_res.$USER
SUMMARY_FILE=./ergoaitest_summary.txt

ERGO="$ERGODIR/$runergo_script"

if test ! $scripttest_flag "$ERGO"; then
    echo "Can't execute $ERGO"
    echo "aborting..."
    echo "Can't execute $ERGO" >$MSG_FILE
    HOSTNAME=`hostname`
    echo "Aborted testsuite on $HOSTNAME..." >> $MSG_FILE
    #mail $USER < $MSG_FILE
    rm -f $MSG_FILE
    exit
fi

lockfile=lock.test
testdir=`pwd`

trap 'rm -f $testdir/$lockfile; exit 1' 1 2 15

if test -f $testdir/$lockfile; then
    cat <<EOF

************************************************************
The lock file ./$lockfile exists.
Probably testsuite is already running...
If not, remove
        ./$lockfile
and continue
************************************************************

EOF
   exit
else
   echo $$ > $lockfile
fi


if test -f "$RES_FILE"; then
  echo "There was an old $RES_FILE"
  echo "removing..."
  rm -f $RES_FILE
fi


LOG_FILE_SIZE=400000
if test -f "$LOG_FILE"; then
    if [ "$(uname)" = "Darwin" ]; then
	LOG_FILE_SIZE=$(stat -f %z "$LOG_FILE")
    else
	LOG_FILE_SIZE=$(stat -c %s "$LOG_FILE")
    fi
    echo "There was an old $LOG_FILE"
    echo "removing..."
    rm -f $LOG_FILE
fi

# if the tests were canceled in the middle, give a reasonable size estimate
if [ $LOG_FILE_SIZE -le 410000 ]; then
    LOG_FILE_SIZE=410000
fi


# should parameterize: create a script that given an input file
# greps for errors and prints the results to an output file
# and then this script can also be used in buildtest

echo "Testing $ERGO"
echo "The log will be left in  $LOG_FILE"

echo "Log for  $ERGO > $LOG_FILE"
echo ""

(echo "Date-Time: `date +"%Y-%m-%d %H:%M"`" >> $LOG_FILE) || status=failed
if test -n "$status"; then
    # it was a Mac
    echo "Date-Time: `date`" >> $LOG_FILE
    BSD_DATE=1
else
    BSD_DATE=0
fi

# test if /usr/bin/time exists and takes format argument
/usr/bin/time -f "" echo > /dev/null 2>&1 && formatted_time=yes

echo Started on `date`
if [ "$formatted_time" = "yes" ]; then
    /usr/bin/time -f "TESTTIME user: %U, elapsed: %E" \
	          ./testall.sh -opts "$options" \
	          -logsize $LOG_FILE_SIZE -logfile $LOG_FILE \
	          -exclude "$excluded_tests" \
		  -add "$added_tests"  \
		  -only "$only_tests"  \
		  $ERGO  >> $LOG_FILE 2>&1
else
    # Mac and the like, which don't have a decent time function
    (TIMEFORMAT="TESTTIME user: %U, elapsed: %E" ; \
	time  \
	    ./testall.sh -opts "$options" \
	    -logsize $LOG_FILE_SIZE -logfile $LOG_FILE \
	    -exclude "$excluded_tests" \
	    -add "$added_tests"  \
	    -only "$only_tests"  \
	    $ERGO  >> $LOG_FILE 2>&1)
fi

touch $RES_FILE
coredumps=`find . -name core -print`

if test -n "$coredumps" ; then
  echo "The following coredumps occurred during this test run:" >> $RES_FILE
  ls -1 $coredumps >> $RES_FILE
  echo "End of the core dumps list" >> $RES_FILE
fi
# check for seg fault, but not for segfault_handler
$GREP "fault" $LOG_FILE | $GREP -v "segfault_handler" | $GREP -v "default" | $GREP -v "pagefault"  >> $RES_FILE
# core dumped
$GREP "dumped" $LOG_FILE >> $RES_FILE
$GREP "Error\[" $LOG_FILE >> $RES_FILE
# when no output file is generated
$GREP "no match" $LOG_FILE >> $RES_FILE
# for bus error
$GREP "bus" $LOG_FILE >> $RES_FILE
# for really bad outcomes
$GREP "missing" $LOG_FILE >> $RES_FILE
# for wrong results
$GREP "differ!" $LOG_FILE >> $RES_FILE
$GREP "different!" $LOG_FILE >> $RES_FILE
# when xsb aborts... it writes something like ! Aborting...
$GREP "abort" $LOG_FILE | $GREP -v "on_abort" >> $RES_FILE
# for overflows (check for Overflow & overflow)
$GREP "overflow" $LOG_FILE >> $RES_FILE
# for ... missing command...
$GREP "not found" $LOG_FILE >> $RES_FILE
#$GREP "abnorm" $LOG_FILE >> $RES_FILE
$GREP "denied" $LOG_FILE >> $RES_FILE
$GREP "no such file" $LOG_FILE >> $RES_FILE
$GREP "illegal" $LOG_FILE | $GREP -v "read_canonical" >> $RES_FILE 
# sometimes after overflow the diff fails and a message with Missing
# is displayed
$GREP "missing" $LOG_FILE >> $RES_FILE
# 
$GREP "fatal" $LOG_FILE >> $RES_FILE
# some other problems that should highlight bugs in the test suite
$GREP "syntax error" $LOG_FILE >> $RES_FILE
$GREP "cannot find" $LOG_FILE >> $RES_FILE
echo "-----------------------------------------"

if test "$BSD_DATE" = 1; then
	NEW_LOG=$LOG_FILE.$BSD_DATE
	cp $LOG_FILE $NEW_LOG
else
	NEW_LOG=$LOG_FILE-`date +"%y.%m.%d-%H:%M:%S"`
	cp $LOG_FILE $NEW_LOG
fi

HOSTNAME=`hostname`

echo "" >> $SUMMARY_FILE
date >> $SUMMARY_FILE
$GREP "Ergo Version" $LOG_FILE | sort -u >> $SUMMARY_FILE
$GREP "TESTTIME" $LOG_FILE >> $SUMMARY_FILE

# -s tests if size > 0
if test -s $RES_FILE; then
	cat $RES_FILE
	echo "-----------------------------------------"
	echo "***FAILED testsuite for $ERGO on $HOSTNAME"
        echo "***FAILED testsuite for $ERGO on $HOSTNAME" > $MSG_FILE
	echo "Check the log file $NEW_LOG" >> $MSG_FILE
	echo "" >> $MSG_FILE
	echo "    Summary of the problems:" >> $MSG_FILE
	echo "" >> $MSG_FILE
	cat $RES_FILE >> $MSG_FILE
	#mail $USER < $MSG_FILE
	$GREP "differ!" $LOG_FILE >> $SUMMARY_FILE
	echo "FAILED" >> $SUMMARY_FILE
	rm -f $MSG_FILE
else
	echo "PASSED testsuite for $ERGO on $HOSTNAME"
	echo "PASSED" >> $SUMMARY_FILE
	rm -f $NEW_LOG
fi

$GREP "TESTTIME" $LOG_FILE

rm -f $RES_FILE
rm -f $lockfile

echo "Finished on `date`"
echo ==============================================================
