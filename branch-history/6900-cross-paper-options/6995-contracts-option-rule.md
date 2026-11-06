---
id: 6995-contracts-option-rule
subject: 'contracts: the contracts option rule []'
depends: [6990-contract-violation-information, 5400-p4451]
regenerates: []
fixes: []
---

## Rationale

The contracts options follow one rule, as in our GCC (notadragon_wg21
DECISIONS.md E175-E178), so that under edg-gxx the front end and the paired
g++ agree, decided in `check_contracts_option_rule` once the language mode
has said whether contracts end up enabled:

* The paper options are additive.  Each `--contracts_pNNNN` already turns
  contracts on (unless `--[no_]contracts` is given), and P4451 is now on
  when given or when P3400 or P3850 ends up on, whatever the order of the
  `--no_` forms (the implication of 5400-p4451, claimed there, says so).
* A paper option given positively and left on is an error when a final
  `--no_contracts` turns contracts off; all of them are named in one error,
  since a command-line error ends the compilation (g++ gives one error per
  option).  One left on only by `--contracts_p3850` (or P4451 by P3400) is
  not named.  The `--no_` forms are always accepted.
* `--no_contracts_p4451` with P3400 on is an error, worded by whether P3400
  was given or came from `--contracts_p3850`.
* With contracts off, `--contracts_allow_assume`,
  `--contract_evaluation_semantic` and `--contract_analysis_semantic` (any
  value) warn that they have no effect, and so do the P4405 strategies
  (`--contracts_prove_fold`, `--contracts_prove_override`,
  `--contract_analysis_skip_dead_calls`) given positively when P4405 ends up
  off; the `--no_` forms do not, nor does `--contract_violation_information`.
  The configuration options keep their own warning.
* The warnings that an evaluation semantic needs `--contracts_p4298` or
  `--contracts_p4405` are deferred and given only when contracts end up on,
  as g++ gives them (with the C-generating back end's rejection of a
  noexcept semantic's replacement they are given at once, before it).  In C,
  where every positive contracts option is already an error, nothing else
  changes.

edg_gxx.sh leaves the "no effect" warnings (tag
`contract_option_no_effect`) to g++ when g++ runs, so each is given once,
and so too every other warning about the contract options that g++ gives
with the same meaning (notadragon_wg21 DECISIONS.md E120): that the
configuration options have no effect without contracts (2400-p3595), that
an evaluation semantic needs `--contracts_p4298` (4800-p4298) or
`--contracts_p4405` (4900-p4405), that an analysis semantic needs
`--contracts_p4405`, and the configuration's own warnings (tag
`contract_configuration`; the others are tagged by their error codes).
Under `-E` and `-fsyntax-only` the front end still gives them all.  When
the front end stops with an error, so that g++ does not compile,
`front_end_failed` runs g++ on an empty file of the name it would have
compiled, with the language standard, the contract options and the options
that control diagnostics, so that it gives them once (none depends on the
source); if g++ rejects those options, the front end has already stopped on
the same command-line error and g++ adds nothing.  The exit status stays
the front end's.  It
also gives g++ `-fcontract-comment-from-expression` only when g++, asked
with the language standard and the contract options, neither rejects it nor
warns that it has no effect (the probe of 1140-p2900-base-driver, claimed
there, uses the arguments gathered here).

The tests are `tests/tests/contracts/optrule/`: GCC's
`contract-options-additive-*` and `contract-options-rule-*` mirrored for
cpfe's options, Clang's configuration-form no-effect case, and
`report_once`, which pins each of those warnings once through edg-gxx,
also when the front end fails.  The
tests of earlier bands whose cases change (a paper option with
`--no_contracts`, P4451's implication, the P4405 strategies and
`--contracts_allow_assume` warning, the updated imports) are claimed whole
by those bands.

## Compile gap

The deferral of the semantic warnings (4800-p4298, 4810-p4298-c-gen,
4900-p4405) calls `defer_contract_semantic_warning` and
`issue_deferred_contract_semantic_warnings`, which this commit adds: the
commits from 4800-p4298 to here lack them (intermediate commits of the
generated branch need not build, DECISIONS.md E2).  Through edg-gxx, those
commits' probe passes an empty `probe_args`, as before this commit.  The full compilation's call of
`front_end_failed`, which this commit adds, is in the generation loop of
1140-p2900-base-driver, so those commits from 1140-p2900-base-driver on
call it before it exists.

## Contents

- src/Changes : /^11\/4\/26 \[\]$/, /^Contracts: the contracts option rule$/, /^As in our GCC, the paper options are additive: --contracts_p3850 and$/, /^on\.  A paper option given positively and left on is an error when a final$/, /^P3400 on\.  With contracts off, --contracts_allow_assume,$/, /^have no effect, as do the P4405 strategies given positively without P4405;$/, /^the --no_ forms and --contract_violation_information do not\.  A semantic$/
- src/cmd_line.c : #5, #10
- src/error_msg.txt : /(^|;)ec_cl_(contracts_option_with_no_contracts|no_contracts_p4451_with_p3(400|850)|contract_option_no_effect);/
- util/edg_gxx.sh : /^# The contracts option rule\.  g\+\+ warns that a contract option/
- tests/tests/contracts/optrule/* : *
- tests/expectations/edg_x86_64/contracts.log : /^optrule\//
