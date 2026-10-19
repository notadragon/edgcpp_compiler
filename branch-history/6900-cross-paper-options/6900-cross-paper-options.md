---
id: 6900-cross-paper-options
subject: 'contracts: name every evaluation semantic in the diagnostics that list them []'
depends: [6000-p3100-core]
regenerates: []
fixes: []
---

## Rationale

What is left of the option work once each paper's lines have gone to the
paper (M15b, DECISIONS.md E29.2): the lines that name several papers at once.
EDG's implication chain is per paper already (each `--contracts_pNNNN`
enables contracts in its own band), so the residue is the four diagnostics
that list the contract evaluation semantics, which named only P2900's ignore,
observe, enforce and quick_enforce after P4298 (noexcept_enforce,
noexcept_observe) and P3100 (assume) had added theirs:

* `ec_cl_invalid_contract_evaluation_semantic` (an unknown
  `--contract_evaluation_semantic`) lists all seven;
* `ec_cl_contract_evaluation_semantic_needs_cp_gen_be`,
  `ec_contract_computed_semantic_unsupported` and the configuration's error
  in `check_entry_semantic` list the five the C-generating back end accepts
  (ignore, quick_enforce, noexcept_enforce, noexcept_observe, assume).

The tests whose expected output names these texts
(`config/c_gen_semantics`, `p4298/nonthrowing_cgen`, `parse/semantic_option`,
`parse/semantic_option_c_gen_be`) are claimed whole by earlier bands, so
their updated `//match_regex:` lines and recordings appear there.

## Compile gap

The commits from 1000-p2900-base-basic to here lack the three messages of
`error_msg.txt` this commit adds (the earlier bands' code names them), and
`check_entry_semantic` lacks its error's text: intermediate commits of the
generated branch need not build (DECISIONS.md E2, as for GCC and Clang).

## Contents

- src/error_msg.txt : /(^|;)ec_cl_(invalid_contract_evaluation_semantic|contract_evaluation_semantic_needs_cp_gen_be);/, /(^|;)ec_contract_computed_semantic_unsupported;/
- src/contract_config.c : @constant:6
