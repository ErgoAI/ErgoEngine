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
    echo "net_size($1)."
    # elemNet reads gen_elem:rule/3, not benchmark_elem's separate rule/3.
    # Time generation of one braid separately, matching generate_net(N) in bnergo.
    # Load both modules first so generation timing excludes compilation/loading.
    # Generation wall time is reported as datum([generate_net],Size,Seconds).
    # The existing benchmark clears tables and reports reachability wall time
    # as datum([elem_net],Seconds).
    "$xsb_command" --nobanner --noprompt --nofeedback --quietload \
        -e "(catch((['benchmark_elem.pl'],
                    loader:load(gen_elem),
                    walltime(GenerationStart),
                    gen_elem:gen_proc_2($1,1),
                    walltime(GenerationEnd),
                    GenerationTime is GenerationEnd-GenerationStart,
                    write(datum([generate_net],$1,GenerationTime)), writeln('.'),
                    flush_output,
                    benchmark_elem:bench_private_process_2(1)),
                   Error, (writeln(Error),fail)) -> halt ; halt(1))."
}

#100 000
run_benchmark 100000
echo "------------------------------"
#200 000
run_benchmark 200000
echo "------------------------------"
#500 000
run_benchmark 500000
echo "------------------------------"
#1 000 000
run_benchmark 1000000
echo "------------------------------"
#2 000 000
run_benchmark 2000000
