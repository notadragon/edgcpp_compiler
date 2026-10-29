# EDG-20: through edg-gxx, a contract violation in constant evaluation is warned about twice under observe

**Status:** Deferred by the user 2026-10-01 (notadragon_wg21 DECISIONS.md
E10); tests accept the second warning.
**Component:** the pairing with g++ (`util/edg_gxx.sh`); interpret.c's
reporting (`finish_contract_evaluation`).
**Found:** M4, 2026-10-01.
**Test:** `tests/tests/contracts/constexpr/observe_gxx.sft.cpp` expects both
warnings.

## Reproducer

    constexpr int f(int x) pre(x > 0) { return x; }
    constexpr int y = f(-1);

    edg-gxx -std=c++26 -fcontract-evaluation-semantic=observe -c t.cpp

warns twice: the front end's `"t.cpp", line 1: warning: contract predicate
is false in constant expression`, and g++'s `t.cpp:1:24: warning: ...` about
the same assertion in the generated code (cpfe-cp puts contract assertions
out as written, and g++ evaluates them again).  Under a terminating semantic
the front end's error stops the compilation before g++ runs.

## Notes

GCC's warning has no -W option of its own, so edg-gxx cannot turn off g++'s
alone (only -w, which silences every g++ warning).  Possible fixes: a GCC
option for that warning, which edg-gxx would pass; or edg-gxx passing g++
-w, if the generated code's g++ warnings are judged redundant with the front
end's.
