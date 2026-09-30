# Generated tax-query regression workload

This suite exercises the successful predicates in
`robert-bug/src/query_tests.ergo`. It is kept separate from the normal
golden-output directories because the generated workload currently lives
outside the ErgoEngine repository.

Run all 321 successful queries:

```sh
ErgoAI-testsuite/tax_query_tests/testsuite.sh
```

It is also an opt-in ErgoAI test group:

```sh
cd ErgoAI-testsuite
./testsuite.sh -add tax_query_tests ../ErgoAI
```

It is not in the default group list because it requires the external generated
workload and launches 321 independent processes.

The current workspace layout is the default. Other layouts can specify both
inputs explicitly:

```sh
ErgoAI-testsuite/tax_query_tests/testsuite.sh \
  --ergo /path/to/ErgoAI/runergo \
  --main /path/to/robert-bug/src/main.ergo
```

For review and diagnosis, the catalog is divided at the historically useful
boundaries:

- batch 1: catalog positions 1--311;
- batch 2: position 312 (`tax_case_40`);
- batch 3: position 313 (`tax_case_41`);
- batch 4: positions 314--376.

Use `--batch 1`, for example, to run only one batch. The 55 predicates in
`known_failures.txt` are omitted because they return `No` even in a fresh
process. The runner checks that the source still has 376 catalog entries and
that exclusion still leaves exactly 321 tests, so changes to the generated
workload cannot silently change coverage.

Each predicate runs in a fresh Ergo/XSB process. This tests independent query
correctness and crash freedom. It intentionally does not replace the
same-process workload, whose cross-query state lifetime is a separate stress
test and remains relevant to the outstanding full-workload investigation.
