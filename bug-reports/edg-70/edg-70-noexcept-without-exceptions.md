# EDG-70: with exceptions disabled, the noexcept operator is true for every expression

**Status:** Open upstream; deferred on this branch by the user 2026-10-03 (EDG behavior here is not to be changed; DECISIONS.md E20 in notadragon_wg21).
**Component:** types.c `is_nothrow_type` (TRUE for every function type
without exceptions), il.c `expr_might_throw` (no traversal without
exceptions); every user of the answer: the noexcept operator, the nothrow
type traits, a requires-expression's `noexcept`.
**Upstream Link:** None found (searched 2026-10-03).
**Affects:** stock EDG at `0ac366374c` in GNU and Clang modes with
`--no_exceptions` (g++'s `-fno-exceptions`).
**Found:** M8 import sweep, 2026-10-03: imported GCC
`noexceptions-contracts` (`noexcept(clamp(1))` must be false).
**Test:** `tests/tests/contracts/openbugs/edg-70-noexcept-without-exceptions.sft.cpp`
records the current behavior.

## Bug Report

With exceptions disabled, `noexcept(f())` is true for a function `f` not
declared noexcept, and so are `std::is_nothrow_*` and friends.  g++ and Clang
answer from the exception specifications whatever `-fno-exceptions` says
(GCC's `expr_noexcept_p` walks the expression regardless of
`flag_exceptions`), so through edg_gxx.sh a program's `if constexpr
(noexcept(...))` or `static_assert` behaves differently from g++.

## Reproducer

[`noexcept-without-exceptions.cpp`](noexcept-without-exceptions.cpp):

    cpfe-cp --gnu_version=170000 --no_exceptions --c++26 --no_code_gen noexcept-without-exceptions.cpp

## Notes

`expr_might_throw` skips its traversal without exceptions, and
`is_nothrow_type` returns TRUE for every type then; both also serve code
generation (cleanups, terminate handlers), where "nothing throws" is right.
A fix needs the language-level questions (noexcept operator, traits,
requires-expressions) to read the exception specifications in GNU and Clang
modes while code generation keeps its answer -- not a one-line change.
