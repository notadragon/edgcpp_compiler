# EDG-73: the C-generating back end puts out a function whose only parameter is `...` as `T f()`

**Status:** Deferred by the user 2026-10-05.
**Component:** c_gen_be.c / targ_def.h (`ALLOW_ELLIPSIS_ONLY_PARAM_IN_GENERATED_C`,
which follows `C_GEN_BE_GENERATES_C23`, which follows the host compiler
that built EDG -- gcc-toolset-13 here).
**Found:** 2026-10-03, by the EDG-67 work (imported GCC `c23-stdarg-split-1b`).
**Test:** `tests/tests/contracts/openbugs/edg-73-ellipsis-only.sft.cpp`
records the failure.

## Reproducer

    int h(...) { return 1; }
    int main() { return h(1, 2) == 1 ? 0 : 1; }

through eccp (the C-generating configuration): the generated C declares
`int h()`, which our gcc 17 (C23 by default, where `()` means `(void)`)
rejects at the call: "too many arguments to function".

## Notes

Not fixable in the front end proper: configure the C-generating back end to
generate C23 (`C_GEN_BE_GENERATES_C23`), or compile eccp's output with
`-std=gnu17`.  The harness choice is the user's.
