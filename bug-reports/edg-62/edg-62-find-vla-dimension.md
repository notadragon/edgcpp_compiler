# EDG-62: lowering a multidimensional VLA with an initializer, or a VLA parameter sized by another, is an internal error

**Status:** Open upstream; not fixed on this branch, deferred by the user
2026-10-03 (row in `../open-issues/`).  Not contracts-specific.
**Component:** lower_c99.c (VLA lowering), the assertion in il.c
`find_vla_dimension`: the array type being lowered has no VLA dimension
entry in the current function.
**Upstream Link:** None found (searched 2026-10-02).
**Affects:** stock EDG at `0ac366374c`, C-generating back end, in the
Release configuration (`linux-gcc-release`).  The Debug configuration
(`simple-conf/debug`, what EDG's recordings come from) writes an IL file
instead of lowering, so edgy never reaches this code there; the imported
tests pass in EDG's recordings.
**Found:** M6 baseline triage, 2026-10-02: imported GCC tests
`init/array24`, `ext/vla19`, `ext/vla24`, `pr86988`, `c/vla-4`, `c/vla-5`.
**Watch test:** none yet (the six imported tests deviate).

## Bug Report

Generating C for either program stops with "internal error: assertion
failed: find_vla_dimension: not found".  Both are valid in GNU mode (VLAs
are an extension in C++, standard in C99).

## Reproducer

[`vla-2d-init.cpp`](vla-2d-init.cpp) (C++, GNU mode; also `int a[][n] =
{ 0 };` and `char b[1][n] = { { "1" } };`):

    cpfe --gnu_version=999999 --gen_c_file_name=out.c vla-2d-init.cpp

[`vla-param-sizeof.c`](vla-param-sizeof.c) (C99):

    cpfe --gcc --c99 --gnu_version=999999 --gen_c_file_name=out.c vla-param-sizeof.c

A one-dimensional VLA with an initializer (`char c[n] = { "1" };`) is
fine.  With `--no_code_gen` nothing is reported.

## Notes (fix attempted 2026-10-03, not small)

Three separate faults, each hiding the next:

* `int a[2][n] = {}`: `vla_dimension_expr_for_type` (lower_c99.c), with
  `LOWER_VARIABLE_LENGTH_ARRAYS`, hands the whole array type to
  `vla_dimension_variable`, but a VLA type with constant outer dimensions
  has no dimension entry for its outer component (the non-lowering branch
  below it loops over them; `vla_size_expr(type, FALSE)` would do it here).
  With that, C generation fails next: "dump_initializer_part: bad entity
  type" -- the lowered variable (a pointer) keeps its aggregate initializer.
* `void f(int a, int b[a][a], int c[sizeof(*b)])`: a dimension expression
  written in a prototype is lowered with `innermost_function_scope` cleared
  (so its temporaries go to file scope; `lower_vla_dimension_expression`),
  which also hides the function's VLA dimension entries from the `sizeof`
  in it.  Restoring them for the lookup leads to an assertion in
  il_to_str.c while C is generated.
* EDG-63 is a third.
Deferred by the user (DECISIONS.md E16).
