//remark:contracts: the contracts option rule (notadragon_wg21 DECISIONS.md E177, E178), as our GCC's contract-options-rule-p4405-sub*: --contracts_prove_fold, --contracts_prove_override and --contract_analysis_skip_dead_calls given positively warn that they have no effect when P4405 ends up off (also with contracts off); the --no_ forms do not, nor does one given before --contracts_p4405
//type:fp
//options:--c++26 --contracts --contracts_prove_fold --contracts_prove_override --contract_analysis_skip_dead_calls:--c++23 --no_contracts_prove_fold --no_contract_analysis_skip_dead_calls --no_contracts_prove_override:--c++23 --contracts_prove_fold:--c++26 --no_contracts_prove_fold --no_contract_analysis_skip_dead_calls --contracts_prove_override --contracts_p4405:--c++26 --contracts_prove_fold --contracts_p3850 --no_contracts_p4405:--c++26 --contracts_p3850 --no_contracts_p4405 --contracts_prove_fold
//match_regex:^(Command-line (error|warning): .*|)$
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-options-rule-p4405-sub*.C
int i;
