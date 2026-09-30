#!/bin/sh

# Run the successful queries in robert-bug/src/query_tests.ergo.  Every query
# gets a new Ergo/XSB process: this suite checks each query independently and
# does not conceal or conflate failures with the separate same-process stress
# workload.

usage()
{
    echo "Usage: $0 [--batch 1|2|3|4|all] [--ergo /path/to/runergo] [--main /path/to/main.ergo]"
}

script_dir=`CDPATH= cd -- "$(dirname -- "$0")" && pwd`
workspace_dir=`CDPATH= cd -- "$script_dir/../../.." && pwd`
ERGO="$script_dir/../../ErgoAI/runergo"
MAIN="$workspace_dir/robert-bug/src/main.ergo"
batch=all

while test $# -gt 0; do
    case "$1" in
        --batch)
            test $# -ge 2 || { usage; exit 2; }
            batch=$2
            shift 2
            ;;
        --ergo)
            test $# -ge 2 || { usage; exit 2; }
            ERGO=$2
            shift 2
            ;;
        --main)
            test $# -ge 2 || { usage; exit 2; }
            MAIN=$2
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            usage
            exit 2
            ;;
    esac
done

case "$batch" in
    1|2|3|4|all) ;;
    *) usage; exit 2 ;;
esac

if test ! -x "$ERGO"; then
    echo "Cannot execute Ergo launcher: $ERGO" >&2
    exit 2
fi
if test ! -f "$MAIN"; then
    echo "Cannot read workload entry point: $MAIN" >&2
    exit 2
fi

query_file=`dirname -- "$MAIN"`/query_tests.ergo
if test ! -f "$query_file"; then
    echo "Cannot read query catalog: $query_file" >&2
    exit 2
fi

tmp_dir=`mktemp -d "${TMPDIR:-/tmp}/ergo-tax-query-tests.XXXXXX"` || exit 2
trap 'rm -rf "$tmp_dir"' 0
trap 'exit 1' 1 2 15
all_queries="$tmp_dir/all-queries"
selected_queries="$tmp_dir/selected-queries"
run_queries="$tmp_dir/run-queries"

sed -n "s/^all_query_test('\([^']*\)').$/\1/p" "$query_file" > "$all_queries"
catalog_count=`wc -l < "$all_queries" | tr -d ' '`
if test "$catalog_count" -ne 376; then
    echo "Query catalog changed: expected 376 entries, found $catalog_count" >&2
    exit 2
fi

position=0
while IFS= read -r query; do
    position=`expr "$position" + 1`
    if grep -F -x -q "$query" "$script_dir/known_failures.txt"; then
        continue
    fi
    if test "$position" -le 311; then
        query_batch=1
    elif test "$position" -eq 312; then
        query_batch=2
    elif test "$position" -eq 313; then
        query_batch=3
    else
        query_batch=4
    fi
    printf '%s %s %s\n' "$position" "$query_batch" "$query" >> "$selected_queries"
done < "$all_queries"

selected_count=`wc -l < "$selected_queries" | tr -d ' '`
if test "$selected_count" -ne 321; then
    echo "Successful-query set changed: expected 321 entries, found $selected_count" >&2
    exit 2
fi

if test "$batch" = all; then
    cp "$selected_queries" "$run_queries"
else
    awk -v wanted="$batch" '$2 == wanted' "$selected_queries" > "$run_queries"
fi

run_count=`wc -l < "$run_queries" | tr -d ' '`
if test "$run_count" -eq 0; then
    echo "No queries selected" >&2
    exit 2
fi

echo "Running $run_count independent tax-query regressions (batch $batch)"
failed=0
completed=0
while read -r position query_batch query; do
    completed=`expr "$completed" + 1`
    output="$tmp_dir/query-output"
    {
        printf "['%s'].\n" "$MAIN"
        printf '%s\n' 'chatter{off}.'
        printf '%s\n' "\\if %$query \\then writeln('QUERY-PASSED')@\\plg \\else writeln('QUERY-FAILED')@\\plg."
        printf '%s\n' '\end.'
    } | "$ERGO" > "$output" 2>&1
    status=$?

    if test "$status" -eq 0 && grep -q 'QUERY-PASSED' "$output" &&
       ! grep -q 'QUERY-FAILED' "$output"; then
        printf 'PASS %3d/%3d  catalog=%3d batch=%s  %s\n' \
            "$completed" "$run_count" "$position" "$query_batch" "$query"
    else
        failed=`expr "$failed" + 1`
        printf 'FAIL %3d/%3d  catalog=%3d batch=%s  %s (status %d)\n' \
            "$completed" "$run_count" "$position" "$query_batch" "$query" "$status"
        tail -n 30 "$output"
    fi
done < "$run_queries"

if test "$failed" -ne 0; then
    echo "$failed of $run_count tax-query regressions failed"
    exit 1
fi

echo "PASSED all $run_count tax-query regressions in batch $batch"
