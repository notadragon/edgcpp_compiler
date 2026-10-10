# EDG-61: 32 IL-display tests pipe their output into commands that do not exist

**Status:** Open upstream; fixed on this branch (band 0100).  Not
contracts-specific; a test bug, not a front end bug.
**Component:** `tests/tests/edg/cmeerw/` (31 tests) and
`tests/tests/edg/ccs/gnats24848.sft.cpp`, their `//filter:` lines.
**Upstream Link:** None found (searched 2026-10-02).
**Affects:** upstream `0ac366374c` and earlier.
**Found:** M6 baseline triage, 2026-10-02: 62 deviations whose recording
held bash's "enumerate_addrs: command not found", whose text differs
between bash 5 (EDG's image) and bash 4.4.
**Test:** the 32 tests themselves.

## Bug Report

31 tests in `edg/cmeerw` (the `*-disp`, `-display`, `-ildisp` and
`gnats-28565-mangling` tests) end their filter with `| enumerate_addrs`,
and `edg/ccs/gnats24848` with `| normalize_test_output`.  Neither is a
command: the tools are `dev_tools/bin/edg-enumerate-il-addrs` and
`dev_tools/bin/edg-normalize-test-output` (`normalize_test_output` is the
name of edgy's in-process filter, used when a test has no `//filter:`).  So
the recorded output of each test is bash's error message, the test checks
nothing, and the recording depends on the shell's wording.

## Reproducer

    grep -rln '| enumerate_addrs\|| normalize_test_output$' tests/tests

lists the 32 tests; each `.rto` recording is the command line plus
`/usr/bin/bash: line 1: enumerate_addrs: command not found`.

## Fix (this branch)

The filters name the tools, and the tests were re-recorded from a Release
build of upstream `0ac366374c`.  A Release build's IL display has fewer
fields than the Debug build EDG records with (`EXTRA_SOURCE_POSITIONS_IN_IL`
and others are Debug-configuration options), so seven recordings, whose
filters select only such fields or entries, are empty; and the
`edg_x86_64_cp` configuration needed its own recordings for 46 cases, whose
`decl_position.seq` values differ.  Upstream would re-record from its Debug
build.  Some filters also contain `\\n` inside an awk regular expression,
which matches a backslash and an `n`, not a newline, so those alternatives
select nothing either way.
