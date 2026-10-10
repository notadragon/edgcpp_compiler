---
id: 9900-fork-metadata
subject: 'Fork metadata: README, bug reports, open issues, branch history'
depends: []
regenerates: []
fixes: []
---

## Rationale

Everything about this branch that is *about* the work rather than part of
it, gathered into the last commit so that dropping one commit leaves a purely
upstreamable series.

`README.md` gains a prepended description of the fork and of how to build
and use it paired with g++.  `branch-history/` is the mapping that generates
this history, including the file you are reading; bands with no commit yet
hold only a README marking them reserved, so that the numbering matches the
GCC and Clang forks.  `open-issues/` is the ledger of what is broken or owed
on this branch, and `bug-reports/` of bugs that reproduce on stock EDG, as in
those forks; each open defect has a watch test in
`tests/tests/contracts/openbugs/`, whose recording is the current behavior.

**A reviewer should read none of this as front end work.**  It is never
upstreamed, and no commit before this one may touch these paths.

## Compile gap

None.

## Contents

- README.md : *
- branch-history/* : *
- open-issues/* : *
- bug-reports/* : *
- tests/tests/contracts/openbugs/* : *
- tests/expectations/edg_x86_64/contracts.log : /^openbugs\//
- tests/expectations/edg_x86_64_gxx/contracts.log : /^openbugs\//
