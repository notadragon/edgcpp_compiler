---
id: 1180-p2900-base-tests
subject: 'contracts: P2900: import GCC and Clang contracts tests []'
depends: [1140-p2900-base-driver]
regenerates: []
fixes: []
---

## Rationale

A first batch of the GCC (`g++.dg/contracts/cpp26`) and Clang
(`clang/test/Contracts`) contracts tests, imported by notadragon_wg21's
`bin/edg/import_tests.py` into `tests/tests/contracts/imported/`: the 340
whose options are only the base facility's (`-std=c++26`, `-fcontracts`,
`-fcontract-evaluation-semantic=`) and that pass, with every expected
diagnostic reported on its line (the importer's `crosscheck`).  Each import
keeps the original test below its EDG directives and runs only under
`edg_x86_64_gxx`, the configuration GCC's and Clang's behavior is compared
with; a test that includes headers beside it is a multi-file test in its
own directory, with copies of them.

The batch is a regression baseline, not a claim of conformance: a test can
pass while exercising something the front end does not yet implement.  The
rest of the suites are imported when the front end can pass them.

## Compile gap

None.  Tests only.

## Contents

- tests/tests/contracts/imported/* : *
- tests/expectations/edg_x86_64/contracts.log : /^imported\//
