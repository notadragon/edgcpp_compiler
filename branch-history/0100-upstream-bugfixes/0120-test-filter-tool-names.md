---
id: 0120-test-filter-tool-names
subject: 'Name the filter tool of edg/ccs/gnats24848 correctly []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG's tests, independent of contracts.
`edg/ccs/gnats24848` pipes its `--il_display` output into
`normalize_test_output`, which is not a command (the tool is
`dev_tools/bin/edg-normalize-test-output`, and `normalize_test_output` is
the name of edgy's in-process filter), so its recording held bash's
"command not found" message, whose text differs between bash versions.  The
filter now names the tool, and the test is recorded with the output it
selects, from a Release build of upstream (whose IL display has fewer fields
than a Debug build's, so the recording is empty).

Tracked in `bug-reports/` (EDG-61) until upstream fixes it.

## Compile gap

None.

## Contents

- tests/tests/edg/ccs/gnats24848.sft.cpp : *
- tests/tests/edg/ccs/.gnats24848.rto/* : *
