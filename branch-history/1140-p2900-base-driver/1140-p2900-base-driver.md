---
id: 1140-p2900-base-driver
subject: 'edg_gxx.sh: contract options []'
depends: [0520-gxx-test-config, 1100-p2900-base-lowering]
regenerates: []
fixes: []
---

## Rationale

Accept g++'s contract options in `edg_gxx.sh`: `-f[no-]contracts` and
`-fcontract-evaluation-semantic=X` map onto the front end's `--[no_]contracts`
and `--contract_evaluation_semantic=X` (the mechanical spelling mapping) and
also go to g++, which checks the contract assertions the front end puts out
as written: g++ needs `-fcontracts` (which `-std=c++26` implies) and the
semantic to generate the checks, link the contracts runtime, and make a
user-defined `::handle_contract_violation` the violation handler.  When g++
accepts `-fcontract-comment-from-expression` (the GCC contracts
implementation's), it is passed as well: the generated code names the
original sources in `#line` directives, but its columns are not theirs, and
g++ would otherwise read a violation's comment from those files at the
generated columns.

edgy's `eccp-gxx` adapter maps the front end spellings used by the tests
back onto g++'s, so that a test's options reach both halves the same way.

A reviewer should check that no contract option reaches only one of the two
compilers.

## Compile gap

None.  Driver scripts only.

## Contents

- util/edg_gxx.sh : #0:1
- util/Changes : @configuration:1, /^edg_gxx\.sh: contract options$/, /^edg_gxx\.sh now accepts g\+\+'s contract options, -f\[no-\]contracts and$/, /^end puts contract assertions out as written, and g\+\+ checks them with the$/, @s:0
- dev_tools/bin/eccp-gxx : #0:1
