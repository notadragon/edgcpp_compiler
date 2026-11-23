# EDG-151: the C++-generating front end requires a copy constructor for `new T(prvalue)`

**Status:** Open upstream; not fixed on this branch, deferred by the user
2026-10-29 (row in `../open-issues/`).
**Component:** expr.c `get_new_routine` (`NEW_CAN_BE_FOLDED_INTO_CTOR`) and
`prep_new_object_init_paren_initializer` -> `scan_ctor_arguments`
(`elision_allowed` is `nps->new_routine != NULL`).
**Upstream Link:** None found (searched 2026-10-29).
**Affects:** stock EDG at `35b7c4776c` (the branch's base), the
C++-generating configuration (`cpfe-cp`, so everything through
`util/edg_gxx.sh`) in a Release build; the C-generating `cpfe` accepts the
program, and so does a Debug build, whose configuration sets
`NEW_CAN_BE_FOLDED_INTO_CTOR=0` (`cmake/macro-conf/support/build-type/debug/base.cmakedef`).
**Found:** AX3e-EDG, 2026-10-29: GCC's
`p4405-analysis-elided-temporary-run.C` (imported to
`imported_p4405/gcc/`) does not compile through edg-gxx.
**Test:** `tests/tests/contracts/openbugs/edg-151-new-prvalue-deleted-copy-ctor.sft.cpp`
records the current behavior (both errors); it requires
`NEW_CAN_BE_FOLDED_INTO_CTOR`, so a Debug run reports REQUIREMENTS NOT MET
against the Release recording.

## Bug Report

A new-expression whose parenthesized initializer is a single prvalue of
the allocated class type -- `new T(T())`, `new T(make())` -- initializes the
object directly from the prvalue ([dcl.init.general]/16.6.1, C++17 and
later): no copy or move constructor is selected, so a deleted or
inaccessible one does not matter.  The C++-generating front end selects the
copy constructor and reports it as deleted.  The braced form `new T{T()}`
is accepted, and so is the same program by the C-generating front end.

## Reproducer

[`new-prvalue-deleted-copy-ctor.cpp`](new-prvalue-deleted-copy-ctor.cpp),
with a stock build:

    cpfe-cp --c++17 --no_code_gen new-prvalue-deleted-copy-ctor.cpp

reports lines 10 and 11 (`EDG_BASE` set as in the build's
`environment.sh`); `cpfe --c++17` reports nothing.

## Notes

Without IL lowering `NEW_CAN_BE_FOLDED_INTO_CTOR` defaults to TRUE
(`targ_def.h`), so `get_new_routine` folds the allocation into the
constructor call (returns a NULL new routine) unless the initializer is
braced, empty or value-initializing.  `scan_ctor_arguments` is then called
with `elision_allowed` FALSE ("the constructor call cannot be eliminated ...
if it's doing the allocation"), which was a permitted optimization before
C++17 but is mandatory elision now.  The initializer is scanned only after
the folding decision, so a fix would either undo the folding once the
operands show a same-class prvalue (when `mandatory_copy_elision` is set)
or not fold when mandatory elision is in effect.  Not contracts-specific.
