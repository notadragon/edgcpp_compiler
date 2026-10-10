# EDG-118: a lambda simple-capture with the ellipsis before the name is accepted

**Status:** Open upstream and on this branch.  Not contracts-specific.
**Component:** class_decl.c (`scan_lambda_capture_list`).
**Upstream Link:** None found (searched 2026-10-09).
**Affects:** stock EDG at `0ac366374c` (cpfe-cp), C++20 and later, with
and without `--strict`.
**Found:** M21-EDG, 2026-10-09, comparing the P3098 capture forms of
GCC's and Clang's `p3098-unexpanded-pack-capture` (EDG-115) with a lambda's.
**Watch test:** `tests/tests/contracts/openbugs/edg-118-lambda-pack-capture-leading-ellipsis.sft.cpp`.

## Bug Report

[expr.prim.lambda.capture]: a simple-capture is `identifier ...opt` (the
ellipsis after the name); only an init-capture puts it first
(`...opt identifier initializer`).  So `[...xs]` is ill-formed, yet

    template <class... T> int g(T... xs) {
      return [...xs] { return (xs + ... + 0); }();
    }
    int a = g(1, 2);

is accepted, and the capture behaves as `[xs...]`.  GCC rejects it, and
Clang says "ellipsis in pack capture must appear after the name of the
capture".

## Reproducer

    cpfe-cp --c++26 --strict --no_code_gen t.cpp     # no diagnostic
