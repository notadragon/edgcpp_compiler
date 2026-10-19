# EDG-52: the operand of an assumption is evaluated after the call in it is inlined

**Status:** Open upstream; fixed on this branch.
**Component:** lower_il.c `lower_statement`, the `stmk_empty` case added by
upstream `55992d9c3a` ("Make the operand of the standard [[assume(...)]]
attribute potentially-evaluated [EDGcpfe/28983]"): it lowers the operand
with `lower_full_expr(expr, statement)`, though `lower_full_expr` takes a
statement only when the expression is that statement's own.
**Upstream Link:** None found (searched 2026-10-01).
**Affects:** stock EDG at `90b66472e0` (Release and Debug builds) and this
branch before the fix.  C-generating back end.
**Found:** 2026-10-02: on this branch as a Debug-only IL failure of a
contracts test (`constexpr/discarded`), whose callee's `contract_assert`
made it eligible for statement inlining.
**Test:** `tests/tests/contracts/codegen/assume_operand.sft.cpp` (fixed here,
so no watch test).

## Bug Report

With the C-generating back end, when the operand of `[[assume(E)]]` has a
call at its top, the call can be inlined by replacing the assumption's
(empty) statement with the inlined code: the operand is then evaluated at
run time, with its side effects, and the attribute is lost.

## Reproducer

[`assume-operand-inlined.cpp`](assume-operand-inlined.cpp):

    cpfe --c++23 --gen_c_file_name=t.int.c assume-operand-inlined.cpp
    gcc -x c t.int.c -o t && ./t; echo $?

prints 1, and `f` in `t.int.c` contains `{ if (__2_15_i == 0) {
++__2_15_i; } ((_Bool)0x1); }`.  The operand of an assumption is not
evaluated ([dcl.attr.assume]); the program must exit 0, as it does with GCC
and Clang.

In a Debug build (with `--set_flag=no_very_expensive_checking`, and ASLR
off as edgy runs it) the lost attribute also leaves the file-scope
`a_scoped_expression` that refers to the operand unreachable when the IL is
written: "IL entry write-read difference ... scoped-expression, written =
1, read = 0" and "read_memory_region: not all expected entries were read".
Without that flag the expensive checks suppress the inlining.

## Fix (this branch)

Pass no statement: `lower_full_expr(expr, (a_statement_ptr)NULL)`.  A call
in the operand can still be inlined within the expression.
