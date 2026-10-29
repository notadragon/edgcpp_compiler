---
id: 0180-c23-va-start
subject: 'Accept __builtin_c23_va_start, the va_start of GCC 15+ headers []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  GCC 15 and later's
`<stdarg.h>` (in C23 mode) and GCC 16 and later's `<cstdarg>` (in C++26
mode) define `va_start(...)` as `__builtin_c23_va_start(__VA_ARGS__)`, which
EDG's GNU emulation did not know: every C++26 program using `va_start`
failed with "identifier "__builtin_c23_va_start" is undefined" (in C, an
implicitly declared function that fails at link time).

The builtin joins `builtin_user_table` (`gc(150000-)g+(160000-)`), entered
when the builtins are preloaded only in C23 and C++26 mode, where GCC makes
it a keyword.  `scan_va_start_operator` takes its form: `(ap)`,
`(ap, last-parameter)`, or `(ap, tokens)` (the tokens skipped, with
parentheses balanced, and warned about, as GCC does), also in a function
whose only parameter is `...`.  It becomes an ordinary `eok_va_start`:
with the last parameter, exactly the classic form; otherwise with a zero
second operand, which GCC's `__builtin_va_start` accepts unchecked.  No IL
or back-end change: both back ends print `__builtin_va_start(ap, 0)`.

Tracked in `bug-reports/` (EDG-67) until upstream fixes it.

## Compile gap

None.

## Contents

- src/sys_predef.h : *
- src/sys_predef.c : *
- src/folding.c : *
- src/expr.c : #1, #2, #3, #4, #5, #6, #7, #8, #56
- tests/tests/contracts/sema/*c23_va_start* : *
- tests/expectations/edg_x86_64/contracts.log : /^sema\/c23_va_start/
