# EDG-190: an IL-display test of the C++-generating front end lacks its NEED_IL_DISPLAY requirement

**Status:** Open upstream (one test); fixed on this branch (band 0100).  Not
contracts-specific; a test bug, not a front end bug.
**Component:** `tests/tests/regressions/unrecognized-attr-arg-pack-expansion/test.mft.cpp`,
its `//require:` lines.
**Upstream Link:** None found (searched 2026-11-23).
**Affects:** upstream `85e38dc288` (which added the test) through
`46ba939329` at least.
**Found:** A3-EDG `--quick`, 2026-11-23: both cases CATASTROPHE under
`edg_x86_64_cp` with a Release build; a Debug build passes.
**Test:** the test itself.

## Bug Report

`regressions/unrecognized-attr-arg-pack-expansion` passes `--il_display`
to the first, C++-generating stage of `cpfe-cp-gen-be` (its
`first_stage_il` script sets `CPFEA_IL_DISPLAY=1`), so it needs a `cpfe-cp`
built with `NEED_IL_DISPLAY`.  It requires only
`RECORD_UNRECOGNIZED_ATTRIBUTES 1`.  The Debug configuration EDG records
with turns `NEED_IL_DISPLAY` on for every front end, but the
`linux-gcc-release` preset turns it on only for `cpfe`
(`cmake/macro-conf/linux-gcc-release/cpfe.cmakedef`, for Compiler Explorer),
not for `cpfe-cp`, so in a Release build each case stops with

    Command-line error: invalid option: --il_display

and its filter selects nothing.  edgy checks `//require:` against the
`--dump_config` of the configuration's front end, here `cpfe-cp` (Release:
`NEED_IL_DISPLAY 0`, `RECORD_UNRECOGNIZED_ATTRIBUTES 1`; Debug: both 1), so
`//require:NEED_IL_DISPLAY 1` (as `edg/ccs/coroutines/gnats24026` has)
makes the test REQUIREMENTS NOT MET where it cannot run.

## Reproducer

In a Release build of stock upstream (`46ba939329`, `bin/edg/build.sh` with
the `linux-gcc-release` preset):

    edgy regressions/unrecognized-attr-arg-pack-expansion

reports `CATASTROPHE--OUTPUT MISMATCH` for both cases under
`edg_x86_64_cp`; running the test's command by hand
(`CPFEA=cpfe-cp CPFEB=cpfe CPFEA_IL_DISPLAY=1 cpfe-cp-gen-be --il_display
...`) prints the command-line error above.

Not undefined behavior: a Debug build (AddressSanitizer) of the same input
is clean, and its filtered IL display equals the recording.

## Fix (this branch)

The test requires `NEED_IL_DISPLAY 1`, and `edg_x86_64_cp`'s expectations
record REQUIREMENTS NOT MET for both cases, as our Release runs report; a
Debug run now shows PASS against that expectation (upstream, recording with
Debug, would keep PASS).  `edg_x86_32_cp`'s expectations are unchanged.
