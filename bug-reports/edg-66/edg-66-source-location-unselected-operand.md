# EDG-66: an immediate invocation in the unselected operand of a constant conditional does not fold its source location default arguments

**Status:** Open upstream; fixed on this branch (band 0100,
`copy_default_arg_expr` in il.c).  Not contracts-specific, but P3290 depends
on it.
**Component:** il.c `copy_default_arg_expr` and `i_copy_expr_tree` (the
`enk_const_eval_deferred` fold); expr.c `scan_conditional_operator`.
**Upstream Link:** None found (searched 2026-10-03).
**Affects:** stock EDG at `0ac366374c`, every configuration.
**Found:** M7 import sweep, 2026-10-03: imported GCC `p3290-assert-basic`
and Clang `cassert-comma-expression` (a contract-integrated `assert(1 == 1)`
expands to `cond ? void(0) : __cxa_handle_cassert_violation(...,
std::source_location::current())`).
**Test:** `tests/tests/contracts/sema/source_location_unselected_operand.sft.cpp`.

## Bug Report

A call of the consteval `std::source_location::current()` (libstdc++'s,
whose default argument is `__builtin_source_location()`) in the unselected
operand of a conditional whose condition is a constant is rejected: "call to
consteval function "std::source_location::current" did not produce a valid
constant expression", with the note "expression cannot be interpreted" at
the default argument.  The operand is potentially evaluated, so the call is
still an immediate invocation, evaluated with its default arguments at the
call site; g++ and Clang accept it.  The same happens with any consteval
function whose default argument is a source location builtin
(`consteval int f(int l = __builtin_LINE())` and `1 ? 0 : f()`).

## Reproducer

[`source-location-unselected-operand.cpp`](source-location-unselected-operand.cpp),
with libstdc++'s headers (g++ 11 or later), in GNU mode:

    cpfe-cp --c++20 --no_code_gen source-location-unselected-operand.cpp

## Cause

`scan_conditional_operator` scans the unselected operand with
`expr_stack->evaluated` FALSE.  When a call there gets its default argument,
`copy_default_arg_expr` replaces the copy options
`CE_COPYING_EVALUATED_DEFAULT_ARG_EXPR` (set because the call is potentially
evaluated) with `CE_COPY_NOT_EVALUATED` (fe76cea011, so that dead code
records no destructions), and only that flag makes `i_copy_expr_tree` fold
the `enk_const_eval_deferred` node wrapping the builtin for the call site.
The wrapper reaches the interpreter, which has no case for it.

## Fix (this branch)

A new copy option, `CE_COPYING_DEAD_DEFAULT_ARG_EXPR`, set with
`CE_COPY_NOT_EVALUATED` for a potentially evaluated call that is not
evaluated; the fold accepts it too.  Destructions and instantiations in dead
code stay as they were.
