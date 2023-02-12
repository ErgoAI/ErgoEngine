#! /bin/sh

# Test files in one directory
#============================================================================
dir=`pwd`
dir=`basename $dir`
echo "-------------------------------------------------------"
echo "--- Running $dir/test_dir.sh                       ---"
echo "-------------------------------------------------------"

basedir=$1
FLORADIR=$2
options=$3

file_list=*.flr

# abp.flr does not work. Perhaps the program is wrong.
# trailer.flr is disabled: since there is no "SINGLE-valued" methods now,
#                          the related trailer is not used.
# btupdates.flr is loaded using btupdates_load.flr
# tabledupdates.flr is loaded using tabledupdates_load.flr
# add1.flr and add2.flr is loaded or added using add_load.flr
# ruleupdates.flr is loaded using ruleupdates_load.flr
# compile_control.flr is loaded using compiletst.flr
# silk-predicates.flr and silk-predicates_at_builtin.flr are auxilary files
# silk imported tests.
# core-pragmatics.flr, core-rules.flr, latest-tiny-kb.flr and rmt-removal-lookup-pos-1.flr
# are main KB for imported tests.
exclude_list="error_nonvar.flr error_nonvar2.flr error_nonvar3.flr \
              silk-predicates.flr silk-predicates_at_builtin.flr \
              core-pragmatics.flr core-rules.flr latest-tiny-kb.flr rmt-removal-lookup-pos-1.flr "

flora_command="_nochatter. %test. _end."

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

OBJEXT=.xwam

# Remove the Prolog and object files to make sure we are using the 
# latest compiled sources
rm -f *.P *${OBJEXT} *.fld *.fdb *.con
rm -f programs/*.P programs/*${OBJEXT} programs/*.fld programs/*.fdb
rm -f gpmanager/*.P gpmanager/*${OBJEXT} gpmanager/*.fld gpmanager/*.fdb
rm -f $basedir/datafiles/*.P    $basedir/datafiles/*${OBJEXT}  $basedir/datafiles/*.fld  $basedir/datafiles/*.fdb
rm -f $basedir/exporttest/*.P    $basedir/exporttest/*${OBJEXT}  $basedir/exporttest/*.fld  $basedir/exporttest/*.fdb
rm -f $basedir/general_tests/prolog/*${OBJEXT}


# run the tests
for file in $file_list ; do
    if member $file "$exclude_list"; then
	continue
    fi
    prog=`basename $file .flr`
    touch $file
    $basedir/test_one_file.sh $FLORADIR/runflora $prog "$flora_command"
done

