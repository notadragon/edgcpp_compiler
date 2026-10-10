---
id: 0120-test-filter-tool-names
subject: 'Name the filter tools of 32 IL-display tests correctly []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG's tests, independent of contracts.  31 tests in
`edg/cmeerw` pipe `--il_display` output into `enumerate_addrs`, and
`edg/ccs/gnats24848` into `normalize_test_output`; neither is a command (the
tools are `dev_tools/bin/edg-enumerate-il-addrs` and
`edg-normalize-test-output`, and `normalize_test_output` is the name of
edgy's in-process filter), so each recording held bash's "command not found"
message, whose text differs between bash versions.  The filters now name the
tools, and the tests are recorded with the output they select, from a Release
build of upstream (whose IL display has fewer fields than a Debug build's, so
seven recordings are empty).

Tracked in `bug-reports/` (EDG-61) until upstream fixes it.

## Compile gap

None.

## Contents

- tests/tests/edg/cmeerw/* : *
- tests/tests/edg/ccs/gnats24848.sft.cpp : *
- tests/tests/edg/ccs/.gnats24848.rto/* : *
