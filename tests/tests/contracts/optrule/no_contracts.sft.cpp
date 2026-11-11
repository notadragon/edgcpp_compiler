//remark:contracts: the contracts option rule (notadragon_wg21 DECISIONS.md E176, E177), as our GCC's contract-options-rule-no-contracts-*: a paper option given positively and left on is an error when a final --no_contracts turns contracts off, whatever the order, named in one error (each command-line error ends the compilation), and one implied by --contracts_p3850 (or P4451 by P3400) is not named; the --no_ forms are always accepted; the last of --contracts and --no_contracts decides, and a later --no_contracts_pNNNN does not turn contracts off while another paper option is on
//type:fp
//options:--c++23 --contracts_p3400 --no_contracts;fc:--c++23 --no_contracts --contracts_p3400;fc:--c++26 --no_contracts --contracts_p3098;fc:--c++23 --contracts_p3097 --contracts_p3098 --contracts_p3099 --contracts_p3100 --contracts_p3290 --contracts_p3400 --contracts_p3850 --contracts_p4283 --contracts_p4298 --contracts_p4301 --contracts_p4405 --contracts_p4451 --no_contracts;fc:--c++26 --no_contracts --no_contracts_p3400 --no_contracts_p3850 --no_contracts_p4451 --no_contracts_p3100:--c++23 --no_contracts --contracts_p3400 --no_contracts_p4451;fc:--c++23 --contracts_p3400 --no_contracts_p3400 --no_contracts:--c++26 --no_contracts --contracts_p3850;fc:--c++23 --no_contracts --contracts_p3400 --contracts:--c++23 --no_contracts --contracts_p4451 --contracts_p3850;fc:--c++23 --contracts_p3098 --no_contracts_p4405
//match_regex:^(Command-line (error|warning): .*|)$
// Case 6: only --contracts_p3400 is named, not --no_contracts_p4451 taking
// part of P3400 away.  Case 11: --no_contracts_p4405 leaves the contracts
// that --contracts_p3098 enables on.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-options-rule-no-contracts-*.C,
// contract-options-rule-contracts-last.C, contract-options-additive-no-contracts.C
#if TEST_NUMBER == 5 || TEST_NUMBER == 7
#ifdef __cpp_contracts
#error contracts enabled despite --no_contracts
#endif
#elif TEST_NUMBER == 9 || TEST_NUMBER == 11
#ifndef __cpp_contracts
#error contracts not enabled
#endif
#endif
#if (TEST_NUMBER == 9) != defined(__cpp_contracts_labels)
#error __cpp_contracts_labels
#endif
int i;
