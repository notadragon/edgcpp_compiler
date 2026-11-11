# EDG-187: through edg-gxx, a precondition on a by-value class parameter is never proven

**Status:** Open (Kind: defect); decided in the paired g++, seen through the
pairing (the EDG front end puts the assertion out as written).  Contracts
are not in upstream EDG.
**Component:** the paired g++ (its analysis evaluations and proof shadow);
edg-gxx.
**Found:** 2026-11-09, D4405R0 implementation examples; measured with the
installed edg-gxx (`79f11b750a`) paired with `~/repos/gnu_gcc-install`.
**Watch test:** `tests/tests/contracts/openbugs/edg-187-byvalue-class-param-not-proven.sft.cpp`.

## What happens

    #include <contracts>
    using namespace std::contracts::labels;
    struct T { int c; };
    int byval(T t)        pre<always_prove>(t.c == 1);
    int byref(const T& t) pre<always_prove>(t.c == 1);
    int scalar(int c)     pre<always_prove>(c == 1);
    int f1() { T a = {1}; return byval(a); }   // diagnosed
    int f2() { return byval(T{1}); }           // diagnosed
    int f3() { T a = {1}; return byref(a); }   // proven
    int f4() { return scalar(1); }             // proven

`edg-gxx -std=c++26 -fcontracts-p3400 -fcontracts-p4405 -O2 -c` reports
"contract assertion cannot be proven" at the calls in `f1` and `f2`, with
the note "the predicate of the contract assertion" at `t.c == 1`.  The same
predicate on a const reference parameter (`f3`) or on a scalar parameter
(`f4`) is proven.

## Expected

No diagnostic: at each call the argument's member `c` is the constant 1, so
the precondition's analysis evaluation is proven, as it is for `f3` and `f4`.

## Provenance

Not stock (contracts are not in upstream EDG) and not the front end's:
with `EDG_GXX_KEEP=1` the C++ that cpfe-cp generates keeps every
declaration, precondition and call as written
(`int byval(T t) pre<(always_prove)>(t.c == 1);`, `return byval(T{1});`),
and our g++ compiling the original source directly gives the same two
diagnostics at the same lines.  Through edg-gxx g++ makes every analysis
evaluation and proof (notadragon_wg21 `edg-overview.md`: decision E7, and
its P4405 rows: "through edg-gxx g++ decides"); the front end's own
analysis diagnostics exist only for the C-generating back end, which
proves nothing at a call (EDG-134, deferred).

## Where the fix belongs

In our GCC (its proof of a precondition over a by-value class-type
parameter at a call).  Nothing is owed in this fork's front end; this row
tracks the behaviour edg-gxx users see.  Once the paired GCC is fixed and
installed, the watch test deviates (the compile succeeds): re-record it as
a passing test (or delete it) and delete this row.
