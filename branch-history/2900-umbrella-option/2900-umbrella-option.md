---
id: 2900-umbrella-option
subject: 'contracts: P3850: add the --contracts_p3850 umbrella option []'
depends: [1000-p2900-base-basic, 1140-p2900-base-driver]
regenerates: []
fixes: []
---

## Rationale

Add `--[no_]contracts_p3850`, one option that turns on every P3850
extension paper this branch implements, as `-fcontracts-p3850` does in the
GCC and Clang contracts implementations.  It exists only because this is a
multi-paper prototype; nothing like it would go upstream.  It enables
contracts unless `--[no_]contracts` is given (unlike GCC, where `.opt`
syntax forces that implication into a later commit, it lands here), and in
C mode it is an error like `--contracts`.  Each paper's own option is
implied unless given explicitly; that implication arrives with the paper,
so here the option enables nothing else yet.

edg_gxx.sh passes `-f[no-]contracts-p3850` to both the front end and g++,
unchanged (DECISIONS.md E18): g++ then turns on all of its papers, and the
front end still rejects the syntax of those it lacks.  edgy's `eccp-gxx`
and `eccp.sh` pass the option on.

Tests: `contracts/p3850/implies` and 7 GCC and Clang imports that need
only the umbrella, in `contracts/imported_p3850/`.

## Compile gap

None.

## Contents

- src/cmd_line.c : #1:2, #2:0, #4:8, #5, #8:1
- src/cmd_line.h : #0:2, /^\t\tcontracts_p3850_enabled;$/
- src/Changes : @functions:1, @implements:0
- src/Changes : /^Contracts \(P3850\): --contracts_p3850$/
- src/Changes : /^The new --\[no_\]contracts_p3850 option enables contracts \(unless$/
- util/edg_gxx.sh : #0:3
- util/eccp.sh : #0:11, #1:8, #2:1
- dev_tools/bin/eccp-gxx : #0:3
- tests/tests/contracts/p3850/* : *
- tests/tests/contracts/imported_p3850/* : *
- tests/expectations/edg_x86_64/contracts.log : /^(p3850|imported_p3850)\//
