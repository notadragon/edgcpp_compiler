# EDG-63: lowering typeof of a dereferenced pointer-to-VLA cast in a comma expression is an internal error

**Status:** Open upstream; not fixed on this branch, deferred by the user
2026-10-03 (row in `../open-issues/`).  Not contracts-specific.
**Component:** lower_c99.c `vla_size_expr` (assertion at line 610 at
`0ac366374c`).
**Upstream Link:** None found (searched 2026-10-02).
**Affects:** stock EDG at `0ac366374c`, C-generating back end, Release
configuration (see EDG-62 on why EDG's recordings do not show it).
**Found:** M6 baseline triage, 2026-10-02: imported Clang test
`c/Sema/vla-2`.
**Watch test:** none yet (the imported test deviates).

## Bug Report

Generating C for a declaration whose type is `__typeof` of the dereference
of a pointer-to-VLA cast inside a comma expression stops with "internal
error: assertion failed at: "lower_c99.c", line 610 in vla_size_expr".

## Reproducer

[`typeof-vla-comma.c`](typeof-vla-comma.c):

    cpfe --clang --clang_version 999999 --c --gen_c_file_name=out.c typeof-vla-comma.c

Clang accepts it (with a warning on the unused left operand).

## Notes (2026-10-03)

`vla_size_expr` asserts that the dimension variable of the VLA component
already exists (`total_number_of_elements`); here it does not when
`lower_vla_decl` asks for it (why the `__typeof` operand's VLA component
got none was not established).  Not small with
EDG-62's faults beside it; deferred by the user (DECISIONS.md E16).
