#! /bin/sh

# Test files in one directory
#============================================================================
dir=`pwd`
dir=`basename $dir`
echo "-------------------------------------------------------"
echo "--- Running $dir/test_dir.sh                           "
echo "-------------------------------------------------------"

LOG_FILE_SIZE=$1
LOG_FILE=$2
basedir=$3
ERGO=$4
options=$5

# if no *.ergo files then echo *.ergo evals to the string '*.ergo' -- set to ""
ergofiles=`echo *.ergo`
if [ "$ergofiles" = "*.ergo" ]; then
    ergofiles=
fi

# if no *.flr files then echo *.flr evals to the string '*.flr' -- set to ""
flrfiles=`echo *.flr`
if [ "$flrfiles" = "*.flr" ]; then
    flrfiles=
fi
file_list="$flrfiles $ergofiles"

# abp.flr does not work. Perhaps the program is wrong.
# trailer.flr is disabled: since there are no "SINGLE-valued" methods now,
#                          the related trailer is not used.
# btupdates.flr is loaded using btupdates_load.flr
# tabledupdates.flr is loaded using tabledupdates_load.flr
# add1.flr and add2.flr are loaded or added using add_load.flr
# ruleupdates.flr is loaded using ruleupdates_load.flr
# compile_control.flr is loaded using compiletst.flr
# depcheck_control.flr is loaded by depchecktest.flr
# error_invalidsym.flr, error_dynmod.flr, error_nonvar*.flr 
#                         are used in export_compile_test.flr
# encap_mod*.flr are used in export_test.flr and importmodule.flr
# encap_syntax_check.flr is used in export_compile_test.flr
#
exclude_list="abp.flr btupdates.flr \
              compile_control.flr compile_control2.flr \
              tabledupdates.flr \
    	      basetype_moo.flr basetype_foo.flr \
              add1.flr add2.flr \
              trailer.flr \
              textual_warn.flr \
              descriptors_errfile.flr \
              template2.ergo \
              template5.ergo \
              tian_skolem.ergo \
              constraints1.ergo \
              alerts.ergo \
              interaction_m1.flr \
              interaction_m2.flr \
              interaction_m3.flr \
              loadloop_aux1.flr \
              loadloop_aux2.flr \
              loadloop_tbox.flr \
              importloop_aux1.flr \
              importloop_aux2.flr \
              importloop_tbox.flr \
              loadudf_err.flr \
              loadudf_noerr.flr \
              loadudf_def.flr \
              sensortest_declarations.flr sensortest_usedecl.flr \
              ruleupdates.flr reif_foo.flr \
              callReified_foo.flr callReified_foo2.flr \
    	      module_foo.flr \
    	      depcheck_control.flr \
              descriptors_moo.flr \
              addf_test_aux.flr \
              structdb_dyn.ergo \
              encap_mod1.flr encap_mod2.flr encap_syntax_check.flr \
              error_invalidsym.flr error_dynmod.flr \
              error_nonvar.flr error_nonvar2.flr error_nonvar3.flr \
              symbol_context_syntax_nowarnings.flr \
              symbol_context_syntax_warnings.flr \
              atco_specific_aux.flr \
	      test_func_modulebar.flr test_func_facts_aux.flr \
	      test_builtin_err.flr \
    	      test_delays_errors1.flr test_delays_errors2.flr \
              test_delays_errors3.flr \
    	      justification_test_fl.flr \
    	      justification_test_prog.flr \
    	      justification_test_prog_aux.flr \
    	      justification_test_just.flr \
              justifier_ex.flr \
              textit1_aux.ergo \
              tnf_data.flr \
              no_newline.flr \
              cranial_data.flr \
              unicode_err.flr \
	      utftest.flr \
              incinc.flr \
              inc.flr \
              inc2.flr \
              regwdemo.ergo \
              regwdemo.ergotxt \
              symwarn1.flr \
              symwarn2.flr \
              maxerr_inc.flr \
              maxerr_data.flr \
              xmlimport_navlinks.flr \
              xmlimport_nonavlinks.flr \
	      unify_reify_tests.flr \
	      udf_id_tests.flr \
              atco_specific2.flr \
              atco_specific2_aux.flr"


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

# Remove Prolog, object, and other auxiliary files to make sure we are using
# the latest compiled sources
rm -f $basedir/datafiles/*.xwam
rm -f $basedir/general_tests/prolog/*${OBJEXT}

rm -f $basedir/apptests/.flora_aux_files/*
rm -f $basedir/apptests/gpmanager/.flora_aux_files/*
rm -f $basedir/apptests/programs/.flora_aux_files/*
rm -f $basedir/datafiles/.flora_aux_files/*
rm -f $basedir/defeasible/*/.flora_aux_files/*
rm -f $basedir/delaytests/.flora_aux_files/*
rm -f $basedir/exporttest/.flora_aux_files/*
rm -f $basedir/functions/.flora_aux_files/*
rm -f $basedir/functions/*/.flora_aux_files/*
rm -f $basedir/general_tests/.flora_aux_files/*
rm -f $basedir/json/.flora_aux_files/*
rm -f $basedir/symbols/.flora_aux_files/*
rm -f $basedir/symbols/*/.flora_aux_files/*

rm -f $basedir/apptests/.ergo_aux_files/*
rm -f $basedir/apptests/gpmanager/.ergo_aux_files/*
rm -f $basedir/apptests/programs/.ergo_aux_files/*
rm -f $basedir/datafiles/.ergo_aux_files/*
rm -f $basedir/defeasible/*/.ergo_aux_files/*
rm -f $basedir/delaytests/.ergo_aux_files/*
rm -f $basedir/exporttest/.ergo_aux_files/*
rm -f $basedir/functions/.ergo_aux_files/*
rm -f $basedir/functions/*/.ergo_aux_files/*
rm -f $basedir/general_tests/.ergo_aux_files/*
rm -f $basedir/json/.ergo_aux_files/*
rm -f $basedir/justifier/.ergo_aux_files/*
rm -f $basedir/symbols/.ergo_aux_files/*
rm -f $basedir/symbols/*/.ergo_aux_files/*


exec 3> /dev/tty
# run the tests
num=1
for file in $file_list ; do
    if member $file "$exclude_list"; then
	continue
    fi
    prog=`basename "$file" .flr`
    prog=`basename "$prog" .ergo`
    touch "$file"
    $basedir/test_one_file.sh "$ERGO" "$prog" 

    num=$(($num+1))
    curr_logsize=$(stat -c %s "$LOG_FILE" 2> /dev/null) || curr_logsize=$(stat -f "%z" $LOG_FILE)
    printf "\rProgress: $((100*$curr_logsize/$LOG_FILE_SIZE))%% " >&3
    if [ $(($num%4)) = 0 ]; then
	printf " | " >&3
    elif [ $(($num%4)) = 1 ]; then
	printf " / " >&3
    elif [ $(($num%4)) = 2 ]; then
	printf " ~ " >&3
    elif [ $(($num%4)) = 3 ]; then
	printf " \\ " >&3
    fi
    printf " %s/%s              "  "$dir" "$file" >&3
done
