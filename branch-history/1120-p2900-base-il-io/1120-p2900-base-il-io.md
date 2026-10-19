---
id: 1120-p2900-base-il-io
subject: 'contracts: P2900: contract assertions in precompiled headers []'
depends: [1090-p2900-base-sema-constexpr, 1100-p2900-base-lowering]
regenerates: []
fixes: []
---

## Rationale

Test that contract assertions survive a precompiled header.  A PCH file is
a dump of the front end's memory regions, and the contract specifiers of a
routine, their result-name variables and their predicates live in
file-scope memory (1020-p2900-base-il), so they need no code of their own:
a declaration read from the PCH keeps its preconditions and postconditions
for constant evaluation, for redeclaration matching, and for the checks
both back ends generate.

The tests are in `tests/tests/contracts/pch`, each compiled twice in PCH
test mode (`--pch --pch_test_mode`; 0520-gxx-test-config teaches the gxx
configuration's adapter that mode), so the second compilation reads the
PCH the first one wrote: constant evaluation and redeclarations of
functions, a function template instantiated only after the PCH, members, a
constructor and a lambda declared in the header; and checks of them under
quick_enforce.  They also pass on a Debug build, which asserts on IL that
points from file-scope memory into a function's.

IL files (`IL_SHOULD_BE_WRITTEN_TO_FILE`, read by `cdisp`) are written and
read through the IL walker, which visits the specifiers
(1020-p2900-base-il); no shipped configuration writes them, so there is no
test here.  Compared by hand for the whole contracts suite, the contract
assertions round-trip unchanged; the files fail on stock constructs
unrelated to contracts (EDG-56).  Contracts across modules are not
supported: EDG's IFC files do not carry them (EDG-54).

## Compile gap

None.  Tests only.

## Contents

- tests/tests/contracts/pch/* : *
