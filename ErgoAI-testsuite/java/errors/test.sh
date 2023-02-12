#! /bin/sh

rm -f actual_output build_output
build.sh > build_output

run.sh > actual_output

echo ""
echo DIFFING ...
echo ""
diff expected_output actual_output
echo ""
echo DONE DIFFING
