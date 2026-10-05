# Prolog transitive-closure benchmarks

From this directory, run:

```sh
sh bench_prolog.sh
sh bench_prolog.sh /path/to/XSB/bin/xsb
```

Without an argument, the runner uses `$XSB` if set, otherwise the engine in
`../../../ErgoAI/.ergo_paths`, falling back to `xsb` on `PATH`. `$XSB` should
contain an executable name/path, not command-line flags.

The 18 runs match `bench_ergo.sh`: chain, cycle, and self-loop, each at 1M,
10M, and 100M, using both ordinary and frame-shaped predicates. Every run
starts a fresh XSB process and uses the shared `edge.pl` graph definitions.
Existing Ergo files and the older `bench_tc.pl` are unchanged.

The six `bench_tc_*.pl` files use ordinary static, variant-tabled Prolog rules.
Tables are cleared before timing; loading/compilation and table clearing are
excluded. Each run prints `time_prolog_tc_<case>(Limit,CPUSeconds,WallSeconds)`.
Ordinary cases stop at the first returned tabled answer, matching the Ergo
runner; frame-shaped cases enumerate all answers, matching the Ergo frame
benchmarks. With the configured local-scheduling XSB, the table is completed
before that first answer is returned. Use the same engine/scheduling for both
languages when comparing timings.

The frame-shaped baseline represents `objid[reachable(Limit,From)->To]` as
`tc_frame_<graph>(objid,reachable(Limit,From),To)`. It preserves the object,
method term, and value shape, but does not implement Ergo's inheritance,
incremental maintenance, explanations, or other frame runtime machinery.
These are lean Prolog baselines, not feature-equivalent Ergo implementations.

The default runs can consume substantial time and memory, especially at 100M.
