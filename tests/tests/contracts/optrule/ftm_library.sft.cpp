//remark:contracts: the contracts option rule (notadragon_wg21 DECISIONS.md E175), as our GCC's contract-options-additive-ftm-*: --no_contracts_p3850 does not turn off the P4451 an explicit --contracts_p3400 implies, nor --no_contracts_p3400 the P4451 --contracts_p3850 implies, whatever the order, so __cpp_contracts and __cpp_lib_contracts are raised and <contracts> declares labels::terminating, with P3400's other labels only with P3400
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++26 --contracts_p3400 --no_contracts_p3850:--c++26 --no_contracts_p3850 --contracts_p3400:--c++26 --contracts_p3850 --no_contracts_p3400:--c++26 --no_contracts_p3400 --contracts_p3850
//use_system_includes: true
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-options-additive-ftm-*.C
#include <contracts>
#if !(__cpp_contracts > 202502L)
#error __cpp_contracts
#endif
#if !(__cpp_lib_contracts > 202502L)
#error __cpp_lib_contracts
#endif
#if !(__gcc_contracts_p4451 > 0)
#error __gcc_contracts_p4451
#endif
#if (TEST_NUMBER <= 2) != defined(__cpp_lib_contracts_labels)
#error __cpp_lib_contracts_labels
#endif
constexpr auto &t = std::contracts::labels::terminating;
#if TEST_NUMBER <= 2
constexpr auto &e = std::contracts::labels::empty_label;
#endif
void f(int x) pre<terminating> (x > 0);
