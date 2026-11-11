---
id: 4960-p4405-callee-premises
subject: 'contracts: P4405: --[no_]contracts_prove_callee_premises []'
depends: [4940-p4405-override]
regenerates: []
fixes: []
---

## Rationale

D4405R0 Section 4.5: an observe or ignore evaluation is a premise that makes
a later proof conditional, also when it is in a callee that the proof
shadow inlines.  In our GCC that is a proving strategy of its own
(`-fcontracts-prove-callee-premises`, implied by `-fcontracts-p4405`): the
proof shadows call hidden premise copies of their callees, which keep the
callees' assumption regions, and an obligation a premise copy proves is
conditional -- never diagnosed, never assumed.

`--[no_]contracts_prove_callee_premises` is that strategy's option, implied
by `--contracts_p4405` (an error in C, like the other contract options); the
drivers map `-f[no-]contracts-prove-callee-premises` to it and pass it on to
g++, which makes every proof through edg_gxx.sh.  The front end proves
nothing, so it does not consult the option.

The tests are `tests/tests/contracts/p4405premises/` (through edg-gxx, the
shapes of D4405R0's bindels example under observe proven, and a
precondition whose predicate calls a function as a premise in its own
function's body and premise copy) and GCC's
premise-copy tests in `tests/tests/contracts/imported_p4405premises/`
(their dump scans and object-code comparisons dropped).

## Compile gap

None: the commit adds its own option.  Intermediate commits of the
generated branch need not build (DECISIONS.md E2, as for GCC and Clang).

## Contents
- src/Changes : /^11\/7\/26 \[\]$/, /^Contracts \(P4405\): --\[no_\]contracts_prove_callee_premises$/, /^The new --\[no_\]contracts_prove_callee_premises option/, /^strategy, to which the drivers pass it on: an observe or ignore evaluation$/, /^in a callee that g\+\+'s proof shadow inlines is a premise of a conditional$/
- src/cmd_line.c : /^  add_option_description\(optk_contracts_prove_callee_premises,$/, /^  if \(option_kind_used\[\(int\)optk_contracts_prove_callee_premises\] &&$/, /^      case optk_contracts_prove_callee_premises:$/, /^  contracts_prove_callee_premises_enabled = FALSE;$/, /^       unless --\[no_\]contracts_prove_callee_premises is given, as in GCC\. \*\/$/
- src/cmd_line.h : /^  optk_contracts_prove_callee_premises,$/, /^\t\tcontracts_prove_callee_premises_enabled;$/
- util/eccp.sh : /^--contracts_prove_callee_premises$/, /^--no_contracts_prove_callee_premises$/, /^         --contracts_prove_callee_premises \| \\$/
- util/edg_gxx.sh : /^# -f\[no-\]contracts-prove-callee-premises goes to both;/, /^    -fcontracts-prove-callee-premises\)$/
- dev_tools/bin/eccp-gxx : /^#   --\[no_\]contracts_prove_callee_premises$/, /^    --contracts_prove_callee_premises\)$/
- tests/tests/contracts/p4405premises/* : *
- tests/tests/contracts/imported_p4405premises/* : *
- tests/expectations/edg_x86_64/contracts.log : /^(p4405premises|imported_p4405premises)\//
