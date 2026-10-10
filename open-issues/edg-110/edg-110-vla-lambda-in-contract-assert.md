# EDG-110: a lambda in a contract_assert predicate that captures a VLA is an internal error

**Status:** Open; this branch only (contracts are not in upstream EDG).
**Component:** the capture of a VLA by a lambda inside a contract-assertion
scope (il.c `find_vla_dimension` asserts).
**Found:** M12-EDG, 2026-10-09, mirroring GCC's
`contract-assert-vla-implicit-capture.C` (EDG-100).
**Watch test:** `tests/tests/contracts/openbugs/edg-110-vla-lambda-in-contract-assert.sft.cpp`.

## Reproducer

    void f(int n) {
      int a[n];
      contract_assert([&] { return a[0] == 0; }());
    }

    edg-gxx -std=c++26 -fsyntax-only t.cpp
    "t.cpp", line 3: internal error: assertion failed: find_vla_dimension: not found

The C++-generating front end (cpfe-cp, as edg-gxx runs it), Release.  The same with `[&a]`, and with
the `contract_assert` in a `[&]` lambda (where the outer lambda should be
diagnosed: the VLA is then used only in the contract assertion).  The same
lambda as the operand of `[[assume]]`, or outside any contract assertion,
compiles, and stock EDG (`0ac366374c`) accepts the `[[assume]]` form, so the
contract-assertion scope is what makes the difference.

## Expected

`f` compiles; the GCC test's `nested_inside` and `nested_inside_after`
(a lambda in the predicate naming the VLA, inside a `[&]` lambda) are
diagnosed as captures for contract assertions only, as `v` is.
