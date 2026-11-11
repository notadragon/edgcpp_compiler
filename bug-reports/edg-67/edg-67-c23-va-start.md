# EDG-67: __builtin_c23_va_start, which GCC 15+ <stdarg.h> uses in C23 and C++26, is unknown

**Status:** Open upstream; fixed on this branch (band 0100, `sys_predef.h`
`builtin_user_table`, expr.c `scan_va_start_operator`).  Not
contracts-specific.
**Component:** sys_predef.h/.c (builtin table and preloading), expr.c
(`scan_va_start_operator`, `scan_c23_va_start_tail`), folding.c.
**Upstream Link:** None found (searched 2026-10-03).
**Affects:** stock EDG at `0ac366374c` in GNU mode with the headers of GCC
15 or later (C23) or GCC 16 or later (C++26).
**Found:** M7 import sweep, 2026-10-03 (GCC `caller-side-variadic`; EDG-11
had listed it as a GCC-only spelling).
**Test:** `tests/tests/contracts/sema/c23_va_start.sft.cpp`.

## Bug Report

GCC 15 and later's `<stdarg.h>` defines, in C23 mode (and in C++26 mode from
GCC 16), `#define va_start(...) __builtin_c23_va_start(__VA_ARGS__)`.  EDG
in GNU mode does not know the builtin, so every such program using
`va_start` fails: "identifier "__builtin_c23_va_start" is undefined" (in C,
an implicitly declared function that fails at link time or in gcc).

## Reproducer

[`c23-va-start.cpp`](c23-va-start.cpp), with g++ 16 or later's headers:

    cpfe-cp --gnu_version=170000 --c++26 --no_code_gen c23-va-start.cpp

## Fix (this branch)

`__builtin_c23_va_start` is a builtin pseudo-call in GNU mode (GCC 15+ in
C, 16+ in C++), entered only in C23 and C++26 mode, as GCC's keyword is.  It
takes `(ap)`, `(ap, last-parameter)` or `(ap, tokens)` (ignored with a
warning, as GCC does), allows a function whose only parameter is `...`,
and becomes an ordinary `eok_va_start`: with the last parameter, the
classic form; otherwise with a zero second operand, which GCC's
`__builtin_va_start` accepts unchecked.  Both back ends print
`__builtin_va_start(ap, 0)` for it.

## Notes

The C-generating back end prints a function whose only parameter is `...`
as `T f()` (`ALLOW_ELLIPSIS_ONLY_PARAM_IN_GENERATED_C` follows
`C_GEN_BE_GENERATES_C23`, which follows the host compiler that built EDG),
which a gcc defaulting to C23 reads as `T f(void)`: EDG-73.
