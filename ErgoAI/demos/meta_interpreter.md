# A staged ErgoAI meta-interpreter

`meta_interpreter.ergo` is a small example to modify, rather than a replacement
for the ErgoAI engine. This first stage interprets declarative queries over
ordinary loaded modules. It retrieves their facts and rules, recursively
interprets rule bodies, and uses ErgoAI's own tabling and negation machinery.
There is no separate encoding of the user's knowledge base.

## Try it

From `ErgoAI/demos`, start `../runergo`, then enter:

```ergo
[meta_interpreter >> meta].
['../../ErgoAI-testsuite/meta_interpreter/data/family.ergo' >> family].
['../../ErgoAI-testsuite/meta_interpreter/data/other.ergo' >> other].

// Direct and interpreted calls both return alice, bob, and carol.
ancestor(alice,?Who)@family.
solve(${ancestor(alice,?Who)@family})@meta.

// Frames, negative rules, and a Boolean combination of goals.
solve(${alice[next->?Who,active]@family})@meta.
solve(${\neg forbidden(?Who)@family})@meta.
solve(${(person(?Who),\naf blocked(?Who))@family})@meta.

// The aggregate body is interpreted; the native operator collects results.
solve(${?N=count{?Who|ancestor(alice,?Who)@family}})@meta.
```

Load the interpreter into a separate module. `solve` takes a **reified formula**,
such as `${p(?X)@kb}`, rather than the HiLog term `p(?X)`. Qualify the goal with
the knowledge-base module; unqualified formulas refer to the module in which
they were written. Arguments remain shared with the goal, so its answer
bindings are returned to the caller. A variable goal obtained from the
knowledge base is interpreted too.

The target is agreement with native **answer sets and truth status** for the
supported declarative fragment. It does not promise native answer order,
proof multiplicity, performance, residual-goal text, or side-effect behavior.

## How it works

1. `solve` materializes any deferred module wrapper and uses `=..` to
   decompose the formula into its kind and arguments.
2. `interpret` handles conjunction, disjunction, and default negation.
   Predicate and frame literals go to `prove_literal`.
3. `prove_literal` tries `isbasefact{Goal}` and enabled rules obtained through
   `clause{...}`. Each rule's body goes back through `solve`. Facts need their
   own branch: ErgoAI's `clause` does not retrieve base facts.
4. Aggregate and conditional operators are reconstructed with interpreted
   subgoals. Their native implementations still control collection, grouping,
   and branching.

`solve` is tabled. This is necessary for left recursion, cyclic rules, and
well-founded undefined answers. Explicit negation is treated as a signed
literal: proving `\neg p(a)` looks for negative facts and rule heads, rather
than testing whether `p(a)` fails.

The companion rule for `\neg solve(Goal)` is necessary because ErgoAI's
`\naf` consults explicit negative evidence. For example, when both `p(a)` and
`\neg p(a)` hold, `\naf p(a)` succeeds. Merely wrapping a Prolog-style failure
test around the positive interpreter would give a different result.

Default negation also carries compiler information about free variables and
existential scope. `=..` alone omits that information. The interpreter uses
`flora_reconstruct_naf_call/3` to keep it and the delay checker while replacing
the positive subgoal with `solve`. Formulas built dynamically without that
checker use the ordinary `\naf solve(...)` path.

## First-stage coverage

| Construct | Treatment |
| --- | --- |
| Relational and HiLog predicates, module-qualified calls | Facts/rules retrieved and interpreted |
| Frames, Boolean methods, membership, subclass queries, signed literals | Explicit facts/rules interpreted; implicit closure delegated |
| Conjunction/disjunction, `\neg`, `\naf`, `\+` | Interpreted with native signed and well-founded negation semantics |
| Existential/universal formulas compiled to Boolean and NAF formulas | Their resulting formulas interpreted |
| Arithmetic, delayed arithmetic, unification, comparisons | Native primitive leaves |
| Aggregates exposed by `=..` | Body interpreted, native collection and grouping |
| `\if ... \then ... [\else ...]`, `\unless ... \do ...` | Subgoals interpreted, native branching |
| Reified goals returned as data | Materialized and interpreted |
| Prolog/library calls and compiler-generated primitive helpers | Native leaves |
| Fact/rule changes and rule enable/disable **between queries** | Looked up in the loaded modules; samples check table refresh |

## Delegation and limits

Inheritance and background axioms are delegated through `\inheritance/2` and
`\bgaxiom/3`, the engine hooks also used by its justifier. A background axiom's
body executes natively. Thus, a subclass closure, inherited attribute, or
equality consequence can contain calls that bypass this interpreter. The
written frame rules themselves still pass through the interpreter. Moving
these implicit derivations into the interpreter is a later stage.

The example is written entirely in ErgoAI but uses a few engine interfaces:
`fllibmodobj/2` for deferred goals, `flora_reconstruct_naf_call/3` for scoped
NAF, `is_flora_library/1` for generated primitive leaves, and the two closure
hooks above. These interfaces are less stable than the ordinary language
constructs and should be checked when porting to another ErgoAI release.

Input programs in this stage must not use argumentation theory, rule defeat,
cancellation, or other defeasible-rule controls. `clause` exposes the written
body, which by itself does not implement the argumentation theory. This is
separate from the native inheritance delegation. Applying an explicitly
defeasible rule raises an unsupported-argumentation exception; this guard
does not provide general support for argumentation modules.

This is a declarative interpreter. Updates **inside an interpreted goal**,
transactional `%` predicates/methods, procedural loops, cut, exception control,
hypotheticals, and every other possible Rulelog/compiler extension are outside
the first stage. Unsupported formula kinds raise
`meta_interpreter_unsupported(Kind,Goal)` rather than silently running the
whole goal natively. Prolog/library leaves can themselves contain effects or
native meta-calls; use pure leaves for answer-equivalence comparisons. Changes
to the knowledge base should be performed outside `solve`, between queries.

Ordinary engine limits still apply, including unsafe negation, nontermination
with infinitely many distinct calls/answers, and the `clause` size limit for
very large rules. The examples validate finite workloads, rather than prove
equivalence for every program in the language. Call-context introspection
(`caller`, tracing information, etc.) can observe interpreter frames and is
also outside the equivalence claim.

## Samples and regression tests

The sample knowledge bases and a comparison driver are in
`ErgoAI-testsuite/meta_interpreter`. From that directory:

```sh
../../ErgoAI/runergo
```

```ergo
[meta_interpreter].
%checks.  // print native/interpreted/expected comparisons
```

Or use the existing suite runner from `ErgoAI-testsuite`:

```sh
bash ./testsuite.sh -only meta_interpreter /absolute/path/to/ErgoAI
```

The driver checks expected answer sets as well as native/interpreted
agreement. It separately checks undefined truth status and exercises updates
and enable/disable operations in one process. Its `%test` entry writes `temp`
for the suite's usual comparison against `meta_interpreter_old`.

## Where to extend it

Start with `prove_literal`: its fact branch and rule-application branch are
the places to introduce an alternative proof policy. An abduction extension
can add a branch for selected literals and thread an explanation state through
the recursive calls. It will need a deliberate table-key and negation policy
for that state; simply adding side-effectful assertions to tabled `solve` is
not sufficient. No abduction behavior is included in this stage.

Add another formula kind by extending both `interpret` and
`interpreted_kind`. Tests should compare answers and truth status with direct
calls and include a case that exercises the construct inside a retrieved
rule body, rather than only at the entry point.
