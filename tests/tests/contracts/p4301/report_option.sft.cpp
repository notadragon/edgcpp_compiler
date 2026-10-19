//remark:contracts: with contracts, --contracts_p4301 (or --contracts_p3850, unless --[no_]contracts_p4301 is given) predefines __cpp_contracts_report, by which libstdc++ declares contract_violation::report(); it enables contracts
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++26:--c++26 --contracts_p4301:--c++26 --contracts_p3850:--c++26 --contracts_p3850 --no_contracts_p4301:--c++23 --contracts_p4301:--c++23 --contracts_p4301 --no_contracts
#if TEST_NUMBER == 2 || TEST_NUMBER == 3 || TEST_NUMBER == 5
#define P4301 1
#endif
#if defined(P4301) != defined(__cpp_contracts_report)
#error __cpp_contracts_report
#endif
#if defined(P4301) && __cpp_contracts_report <= 0
#error the value
#endif
#if TEST_NUMBER >= 5 && (TEST_NUMBER == 5) != defined(__cpp_contracts)
#error contracts not enabled by --contracts_p4301
#endif
#ifdef __cpp_contracts
#include <contracts>
#if defined(P4301) != defined(__cpp_lib_contracts_report)
#error __cpp_lib_contracts_report
#endif
#endif
#ifdef P4301
void handle_contract_violation(const std::contracts::contract_violation &v) {
  static_assert(noexcept(v.report()));
  (void)v.report();
}
#endif
