#!/bin/sh

# Adapter for ErgoAI-testsuite/testall.sh. The ordinary directory runner is
# file/golden-output based; this generated workload has its own query runner.

basedir=$3
ERGO=$4
case "$ERGO" in
    /*) ;;
    *) ERGO="$basedir/$ERGO" ;;
esac

echo "-------------------------------------------------------"
echo "--- Running tax_query_tests/test_dir.sh             ---"
echo "-------------------------------------------------------"

if ./testsuite.sh --ergo "$ERGO"; then
    echo "tax_query_tests tested"
    echo ""
else
    echo "tax_query_tests differ!!!"
    echo ""
fi
