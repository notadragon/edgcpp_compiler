# EDG-65: through edg-gxx, a column-qualified "location" entry of a contract configuration does not match at run time

**Status:** Deferred by the user 2026-10-03 (notadragon_wg21 DECISIONS.md
E17): the deviation is accepted.
**Component:** the pairing with g++ (`util/edg_gxx.sh`, `cp_gen_be.c`'s
`#line` directives); P3595 (`contract_config.c`).
**Found:** M7, 2026-10-03.
**Test:** `tests/tests/contracts/openbugs/edg-65-location-column/` records
the trap.

## Reproducer

    int f(int x)
                        pre(x > 0) { return x; }     // line 15, column 21
    int main() { f(0); }

    edg-gxx -std=c++26 -fcontract-evaluation-semantic=quick_enforce \
      '-fcontract-configuration=[{"match":{"location":"t.cpp:15:21"},
                                  "output":{"semantic":"ignore"}}]' t.cpp

The entry should make the precondition ignored; the program traps.  With
`"t.cpp:15"` (no column) it is ignored.

## Notes

g++ chooses the run-time semantics of the checks it generates, matching the
configuration against the generated code.  cpfe-cp's `#line` directives give
it the original file and line, but its columns are the generated code's.
The front end matches the original column, so constant evaluation (which the
front end does) is right.  A fix would make cp_gen_be.c put each contract
assertion out at its original column, or pass g++ the original positions
some other way; E10 rules out GCC extensions for shaping the pairing's
output.
