#! /bin/sh

rm -f build_output
buildExample.sh fooExample > build_output
buildExample.sh flogicbasicsExample >> build_output

rm -f result_* sorted_*
runExample.sh fooExample > result_fooExample
runExample.sh flogicbasicsExample > result_flogicExample

sort result_fooExample > sorted_result_fooExample
sort result_flogicExample>  sorted_result_flogicExample
sort expected_answer_fooExample.txt>  sorted_expected_answer_fooExample
sort expected_answer_flogicExample.txt >  sorted_expected_answer_flogicExample

echo ""
echo DIFFING ...
echo ""
diff sorted_result_fooExample sorted_expected_answer_fooExample
diff sorted_result_flogicExample sorted_expected_answer_flogicExample
echo ""
echo DONE DIFFING
