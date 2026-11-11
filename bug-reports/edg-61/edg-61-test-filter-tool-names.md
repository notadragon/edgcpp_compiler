# EDG-61: an IL-display test pipes its output into a command that does not exist

**Status:** Open upstream (one test); fixed on this branch (band 0100).  Not
contracts-specific; a test bug, not a front end bug.
**Component:** `tests/tests/edg/ccs/gnats24848.sft.cpp`, its `//filter:`
line.
**Upstream Link:** None found (searched 2026-10-02).
**Affects:** upstream `35b7c4776c` and earlier.
**Found:** M6 baseline triage, 2026-10-02: 62 deviations whose recording
held bash's "command not found", whose text differs between bash 5 (EDG's
image) and bash 4.4.
**Test:** the test itself.

## Bug Report

`edg/ccs/gnats24848` ends its filter with `| normalize_test_output`, which
is not a command: the tool is `dev_tools/bin/edg-normalize-test-output`
(`normalize_test_output` is the name of edgy's in-process filter, used when
a test has no `//filter:`).  So the recorded output of the test is bash's
error message, the test checks nothing, and the recording depends on the
shell's wording.

## Reproducer

    grep -rln '| normalize_test_output$' tests/tests

lists the test; its `.rto` recording is the command line plus
`/usr/bin/bash: line 1: normalize_test_output: command not found`.

## History

As found, 31 tests in `edg/cmeerw` had the same defect (`| enumerate_addrs`
for `dev_tools/bin/edg-enumerate-il-addrs`), and this branch fixed all 32.
Upstream fixed the 31 itself in `21065e551e` ("Fix filter command escaping
and script name", 2026-10-09), also turning the filters' `\\n` into `\n`,
and re-recorded them from its Debug build; the 2026-10-24 rebase took
upstream's version and dropped ours.  `gnats24848` was not part of
upstream's fix.

## Fix (this branch)

The filter names the tool, and the test was re-recorded from a Release
build of upstream `0ac366374c`.  A Release build's IL display has fewer
fields than the Debug build EDG records with, so the recording is empty
(the filter selects `initializer_range` lines, which it does not show); upstream
would re-record from its Debug build.
