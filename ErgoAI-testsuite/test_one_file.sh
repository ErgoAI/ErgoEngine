#! /bin/sh

# test single file

# $1 is expected to have the ERGO invocation script
ERGO=$1
FILE=$2

DIR=`pwd`
BASEDIR=`basename $DIR`

# touching so that in case XSB dumps core we'll have a *_new file and
# a discrepancy will be reported
rm -f ${FILE}_new
touch ${FILE}_new temp

echo "--------------------------------------------------------------------"
echo "Testing $BASEDIR/$FILE"

$ERGO << EOF || outcome=bad
[$FILE].
chatter{off}.
%test.
\end.
EOF

# print out differences.
if [ "$outcome" = bad ]; then
    echo "CRASH OCCURRED" >> temp
fi

cp   temp ${FILE}_new
sort temp > temp_new
sort ${FILE}_old > temp_old

#-----------------------
# print out differences.
#-----------------------
status=0
# must use diff -w to ignore ^M^M under windows
diff -w temp_old temp_new || status=1
if test "$status" = 0 ; then 
	echo "$BASEDIR/$FILE tested"
	echo ""
	rm -f ${FILE}_new
else
	echo "$BASEDIR/$FILE differ!!!"
	echo ""
	diff -w ${FILE}_new ${FILE}_old
fi

rm -f temp temp_old temp_new
