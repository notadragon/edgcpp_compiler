---
id: 0197-test-il-display-requirement
subject: 'Require NEED_IL_DISPLAY in regressions/unrecognized-attr-arg-pack-expansion []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG's tests, independent of contracts.
`regressions/unrecognized-attr-arg-pack-expansion` gives `--il_display` to
the C++-generating first stage of `cpfe-cp-gen-be`, which needs a `cpfe-cp`
built with `NEED_IL_DISPLAY`.  The Debug configuration has it; the
`linux-gcc-release` preset turns it on only for `cpfe`, so in a Release
build both cases stopped with "invalid option: --il_display" (a
CATASTROPHE under `edg_x86_64_cp`).  The test now requires
`NEED_IL_DISPLAY 1`, and `edg_x86_64_cp`'s expectations record
REQUIREMENTS NOT MET for both cases, as a Release build reports.

Tracked in `bug-reports/` (EDG-190) until upstream fixes it.

## Compile gap

None.

## Contents

- tests/tests/regressions/unrecognized-attr-arg-pack-expansion/test.mft.cpp : *
- tests/expectations/edg_x86_64_cp/regressions.log : /^unrecognized-attr-arg-pack-expansion\//
