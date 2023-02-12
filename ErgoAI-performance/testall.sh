#! /bin/sh 

## File:      testall.sh -- test several directories
## Author(s): XSB team
## Contact:   kifer@cs.stonybrook.edu
## 
## Copyright (C) by
##      The Research Foundation of the State University of New York, 2002-2008.
## 
## All rights reserved.
## 
##


echo "-------------------------------------------------------"
echo "--- Running testall.sh                              ---"
echo "-------------------------------------------------------"

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
#     *-only*)
#	    shift
#	    only_tests=$1
#	    shift
#	    ;;
      *)
	    break
	    ;;
    esac
done

if test -z "$1" -o $# -gt 1; then
  echo "Usage: testall.sh [-opts opts] [-exclude \"excl_dirs\"] [-add \"added_tests\"] floradir"
  echo "where: opts       -- options to pass to PROLOG executable"
  echo "       excl_dirs  -- quoted, space-separated list of tests to NOT run"
  echo "       add_dirs   -- list of additional dirs in which to run tests"
  echo "       floradir   -- full path name of FLORA-2 installation directory"
  exit
fi

FLORADIR=$1

# Test if element is a member of exclude list
# $1 - element
# $2 - exclude list
member ()
{
    for elt in $2 ; do
	if test "$1" = "$elt" ; then
	    return 0
	fi
    done
    return 1
}


# float_tests: don't pass. --mk
# regmatch_tests: don't pass on solaris
#default_testlist="general_tests imported_tests apptests exporttest defeasible/new_gclp defeasible/old_gclp defeasible/atck1 defeasible/atck1alt defeasible/atck2 defeasible/atck2alt"

default_testlist="cyc150 "

testlist="$default_testlist $added_tests"

# Run each test in $testlist except for the tests in $excluded_tests
for tst in $testlist ; do
  if member "$tst" "$excluded_tests" ; then
    continue
  else
    currdir=`pwd`
    cd $tst
    if test -f core ; then
	rm -f core
    fi
    $currdir/test_dir.sh $currdir "$FLORADIR" "-e segfault_handler(warn). $options"
    cd $currdir
  fi
done
