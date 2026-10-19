# EDG-58: typeid of a polymorphic glvalue whose dynamic type is known is treated as an unevaluated operand

**Status:** Open upstream; fixed on this branch (band 0100,
`operand_evaluated` in `scan_typeid_operator`).
**Component:** expr.c, the scan of a typeid operand: when the type of the
complete object can be determined (`operand_complete_object_type`),
`runtime_case` is cleared, which also makes the operand unevaluated.
**Upstream Link:** None found (searched 2026-10-02).
**Affects:** stock EDG at `90b66472e0` (`cpfe --c++20` in GNU mode) and
this branch.
**Found:** M5 import sweep, 2026-10-02: GCC's and Clang's
`post-odr-use-template-shapes` (a value parameter used in a postcondition
only as `typeid(x)` was not required to be const).
**Test:** `tests/tests/contracts/sema/typeid_evaluated_operand.sft.cpp`.

## Bug Report

The operand of `typeid` is unevaluated unless it is a glvalue of
polymorphic class type ([expr.typeid]/3-4), and a variable named in a
potentially-evaluated expression is odr-used ([basic.def.odr]).  EDG decides
whether to look the type up at run time, and makes the operand unevaluated
when it does not need to -- including when the complete object's type is
statically known, as for a variable of class type.  Such a variable is then
not odr-used: a lambda names it without capturing it.

## Reproducer

[`typeid-known-type-capture.cpp`](typeid-known-type-capture.cpp):

    cpfe --c++20 --gnu_version=170000 <g++ include paths> typeid-known-type-capture.cpp

accepts `f`, whose lambda odr-uses the parameter `x` without capturing it,
and rejects `g` ("an enclosing-function local variable cannot be referenced
in a lambda body unless it is in the capture list").  Clang rejects both;
our GCC accepts `f` too.

## Notes

The contracts rule that a value parameter a postcondition odr-uses must be
const is checked by a walk of the predicate's IL
(`find_postcondition_param_use`, exprutil.c), which on this branch treats a
non-dynamic `enk_typeid` whose operand is a polymorphic glvalue as
evaluated.  The fix here keeps the two apart: the operand of a polymorphic glvalue is
always scanned as evaluated (`operand_evaluated`), and only the run-time
lookup (`runtime_case`, the typeid node's `is_dynamic`) is skipped when the
complete object's type is known.  The postcondition walk still checks the
operand of a non-dynamic `enk_typeid` itself.

The same shortcut also lost the operand's side effects at run time: with
the C-generating back end, `lower_typeid` replaced a non-dynamic typeid
with the typeinfo object, so `typeid(f(), d)` (`d` a `D` object) never
called `f`.  An operand with side effects is now evaluated ahead of it
(test `sema/typeid_evaluated_operand_run`).  The C++-generating back end
prints the operation as written, so g++ evaluates it.
