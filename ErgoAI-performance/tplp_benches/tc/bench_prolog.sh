#!/bin/sh
# Run from this directory, like bench_ergo.sh.
# Usage: sh bench_prolog.sh [path/to/xsb]
set -e

if test "$#" -gt 1; then
    echo "Usage: sh bench_prolog.sh [path/to/xsb]" >&2
    exit 1
fi

if test "$#" -eq 1; then
    xsb_command=$1
elif test -n "${XSB:-}"; then
    xsb_command=$XSB
elif test -r ../../../ErgoAI/.ergo_paths; then
    # Compare against the same engine used by runergo.
    . ../../../ErgoAI/.ergo_paths
    xsb_command=$PROLOG
else
    xsb_command=xsb
fi

if ! command -v "$xsb_command" >/dev/null 2>&1; then
    echo "Cannot execute XSB: $xsb_command" >&2
    exit 1
fi

# Match runergo's stack limit.
ulimit -s unlimited 2>/dev/null || ulimit -s hard 2>/dev/null || true

run_benchmark()
{
    "$xsb_command" --nobanner --noprompt --nofeedback --quietload \
        -e "(catch((['$1.pl'], $2($3,1)), Error, (writeln(Error),fail)) -> halt ; halt(1))."
}

#---------------------------
# chain
#1 000 000
run_benchmark bench_tc_chain bench_chain 1000000

#10 000 000
run_benchmark bench_tc_chain bench_chain 10000000

#100 000 000
run_benchmark bench_tc_chain bench_chain 100000000

#---------------------------
# cycle
#1 000 000
run_benchmark bench_tc_cycle bench_cycle 1000000

#10 000 000
run_benchmark bench_tc_cycle bench_cycle 10000000

#100 000 000
run_benchmark bench_tc_cycle bench_cycle 100000000

#---------------------------
# self-loop
#1 000 000
run_benchmark bench_tc_self_loop bench_self_loop 1000000

#10 000 000
run_benchmark bench_tc_self_loop bench_self_loop 10000000

#100 000 000
run_benchmark bench_tc_self_loop bench_self_loop 100000000

#---------------------------
# Frame-shaped chain
#1 000 000
run_benchmark bench_tc_frame_chain bench_frame_chain 1000000

#10 000 000
run_benchmark bench_tc_frame_chain bench_frame_chain 10000000

#100 000 000
run_benchmark bench_tc_frame_chain bench_frame_chain 100000000

#---------------------------
# Frame-shaped cycle
#1 000 000
run_benchmark bench_tc_frame_cycle bench_frame_cycle 1000000

#10 000 000
run_benchmark bench_tc_frame_cycle bench_frame_cycle 10000000

#100 000 000
run_benchmark bench_tc_frame_cycle bench_frame_cycle 100000000

#---------------------------
# Frame-shaped self-loop
#1 000 000
run_benchmark bench_tc_frame_self_loop bench_frame_self_loop 1000000

#10 000 000
run_benchmark bench_tc_frame_self_loop bench_frame_self_loop 10000000

#100 000 000
run_benchmark bench_tc_frame_self_loop bench_frame_self_loop 100000000
