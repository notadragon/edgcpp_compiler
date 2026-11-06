//remark:contracts: the contracts option rule (notadragon_wg21 DECISIONS.md E177, E178), as our GCC's contract-options-rule-no-effect-*: with contracts off (C++23 with no option enabling them, or a final --no_contracts), --contracts_allow_assume, --contract_evaluation_semantic and --contract_analysis_semantic warn that they have no effect, a value option for any value, its default included, and the configuration options with their own warning; the --no_ forms do not warn, nor does --contract_violation_information; the warning that a semantic needs a paper option is given only with contracts on
//type:fp
//options_sep:!
//options:--c++23 --contracts_allow_assume --contract_evaluation_semantic=quick_enforce --contract_analysis_semantic=ignore!--c++23 --contracts --contracts_allow_assume --contract_evaluation_semantic=quick_enforce --contract_analysis_semantic=ignore!--c++26 --no_contracts --contract_evaluation_semantic=quick_enforce --contract_analysis_semantic=ignore!--c++23 --contracts_allow_assume --no_contracts_allow_assume!--c++26 --no_contracts_allow_assume --no_contracts!--c++23 --no_contracts_allow_assume!--c++23 --no_contracts_allow_assume --contracts_allow_assume!--c++26 --contracts_allow_assume --no_contracts --contract_evaluation_semantic=quick_enforce --contract_group_evaluation_semantic=g:quick_enforce!--c++26 --no_contracts '--contract_configuration=[]' --contract_configuration_file=no-such-file.json!--c++26 --no_contracts --contract_violation_information=none!--c++23 --contract_evaluation_semantic=prove!--c++26 --contract_evaluation_semantic=prove
//match_regex:^(Command-line (error|warning): .*|)$
// Case 3: ignore is --contract_analysis_semantic's default (the semantics
// are those the C-generating back end accepts).  Case 9: the configuration
// file is not read.  Case 12: with contracts on, prove is warned about as
// before.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-options-rule-no-effect*.C,
// clang/test/Contracts/contract-options-rule-no-effect-no-contracts.cpp
int i;
