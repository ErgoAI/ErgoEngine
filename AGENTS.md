# ErgoAI Engine Repository Guide

## Scope

These instructions apply to the `ErgoEngine` Git repository. Paths below are
relative to this repository root unless stated otherwise.

ErgoAI uses XSB as its execution engine, but XSB is maintained in a separate
repository. In the usual workspace layout it is at `../xsb-code`; consult
`../xsb-code/AGENTS.md` before changing it. Do not modify the sibling XSB
repository unless the task explicitly includes XSB changes.

## Repository Map

- `ErgoAI/`: compiler, command-line runtime, libraries, packages, demos, Java
  integration, installation scripts, and manuals.
- `ErgoAI-testsuite/`: functional and regression tests with checked-in golden
  output.
- `ErgoAI-performance/`: larger performance workloads; do not substitute these
  for correctness tests.
- `ErgoAI-website/`: website and tutorial material.
- `README.md`: short repository overview and links to installation guidance.

The main manuals are under `ErgoAI/docs/`. A supplemental language summary may
be present one level above the repository as `../ErgoAI Guide for LLMs.md`; use
it for orientation, but treat the source and checked-in manuals as authoritative.

## Change Discipline

- Check `git status --short` before editing and preserve unrelated user changes.
- Keep changes narrowly scoped. Avoid broad reformatting or line-ending churn.
- Follow the surrounding file's indentation, naming, and comment style. There
  is no repository-wide formatter.
- Keep copyright headers and substantial file comments intact.
- Search for existing predicates, modules, tests, and conventions before adding
  new ones.
- Treat generated and ignored files as build products, not source. In
  particular, do not edit or commit `.xwam`, generated `.P` files, `*_new` test
  output, temporary logs, `.ergo_paths`, or auxiliary build directories.
- Do not overwrite golden `*_old` test output merely to make a failure pass.
  Inspect the semantic difference and update a golden file only when the
  behavior change is intentional.

## Source Conventions

- Ergo/Flora source commonly uses `.ergo`, `.flr`, `.fli`, and `.flh`.
- XSB Prolog source uses `.P`; `.H` files are Prolog headers, and `.xwam` files
  are compiled artifacts.
- Shell scripts are generally POSIX `sh`; do not introduce Bash-only syntax
  into a `#!/bin/sh` script.
- The repository also contains smaller amounts of C and Java. Match the local
  brace and indentation style rather than imposing a new one.
- Prefer descriptive predicate and module names consistent with nearby code.

## XSB Configuration

`ErgoAI/runergo` sources the generated `ErgoAI/.ergo_paths` file to locate XSB.
A configured checkout therefore needs an executable XSB and a valid
`.ergo_paths`. Do not commit machine-specific absolute paths from that file.

When invoking the ErgoAI Makefile directly, pass the XSB executable explicitly;
its default `PROLOG=none` is intentionally invalid:

```sh
cd ErgoAI
make PROLOG=/absolute/path/to/XSB/bin/xsb base
```

In the usual sibling-checkout layout, the executable is often
`../../xsb-code/XSB/bin/xsb` when invoked from `ErgoAI/`, but do not assume that
layout in scripts or committed configuration.

## Build and Run

Run builds from `ErgoAI/` and pass `PROLOG` as described above:

```sh
make PROLOG=/absolute/path/to/XSB/bin/xsb base    # core ErgoAI
make PROLOG=/absolute/path/to/XSB/bin/xsb nodocs  # libraries/packages, no manuals or demos
make PROLOG=/absolute/path/to/XSB/bin/xsb         # full build, including manuals and demos
```

The default build may update `ErgoAI/version.flh`; review that diff and do not
silently discard a pre-existing user change. `make clean` removes generated
files and also clears Ergo/Flora cache files under the user's `.xsb` directory,
so run it only when the task requires a clean rebuild.

Start a configured command-line session from the repository root with:

```sh
ErgoAI/runergo
```

End an interactive ErgoAI session with `\halt.`. For automation, prefer the
existing noninteractive options supported by `runergo` rather than scripting an
interactive prompt.

## Testing

The primary correctness suite is `ErgoAI-testsuite`. It requires a configured,
executable `ErgoAI/runergo`.

Run the full suite:

```sh
cd ErgoAI-testsuite
./testsuite.sh ../ErgoAI
```

Run one or more test groups while iterating:

```sh
cd ErgoAI-testsuite
./testsuite.sh -only "general_tests" ../ErgoAI
./testsuite.sh -only "general_tests functions" ../ErgoAI
```

The runner writes its main log to `/tmp/ergoai_test_log.$USER`, records a local
summary in `ErgoAI-testsuite/ergoaitest_summary.txt`, and leaves `*_new` output
for mismatches. Read the log and compare `*_new` with the corresponding `*_old`;
do not report success solely from the process exit status.

Testing expectations:

- Run the narrowest relevant group during development.
- Run the full suite for compiler, parser, runtime, shared-library, or other
  broadly visible changes when practical.
- Add or update a regression test for a bug fix or observable behavior change.
- For documentation-only changes, build the affected documentation when the
  necessary toolchain is available; otherwise state that it was not built.
- For shell changes, at minimum run `sh -n path/to/script` in addition to the
  relevant behavioral test.
- Use `ErgoAI-performance` only when the task concerns performance or asks for
  benchmark evidence.

If a change also modifies the sibling XSB repository, follow its `AGENTS.md`,
run the relevant XSB tests there, and then run the affected ErgoAI tests here.

### Generated tax-query workload

`ErgoAI-testsuite/tax_query_tests/testsuite.sh` runs the 321 predicates in the
external `robert-bug` workload that currently succeed independently. It uses a
fresh Ergo/XSB process for each predicate and verifies the expected catalog
size, so it is a correctness and crash-freedom regression rather than a
replacement for the same-process stress workload. Its README documents the
four batch boundaries and how to supply a workload path outside the usual
sibling layout.

When investigating the same-process workload, preserve query order and report
the first failing catalog position. A fresh-process pass does not rule out
stale tables, delay elements, incremental-dependency state, answer tries, or
choice points retained across updates. Diagnose impossible WAM tags and opcode
inputs as evidence of upstream corruption; do not patch the observing
instruction unless its own invariant is demonstrably wrong.

## Commits and Reviews

- Use a concise, imperative commit subject and keep each commit focused.
- In a pull request or handoff, summarize the behavior change, list the exact
  validation commands and results, and explicitly note skipped tests.
- Include relevant configuration details, especially the XSB build or
  configuration tag, when results depend on them.
- Do not commit generated logs, caches, compiled artifacts, or machine-specific
  configuration.
