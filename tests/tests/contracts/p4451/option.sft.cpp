//remark:contracts: --contracts_p4451 (or --contracts_p3400 or --contracts_p3850, whatever the --no_ forms of the other, unless --[no_]contracts_p4451 is given, as in GCC) raises __cpp_contracts to 202611L (or P3097's greater value, with --contracts_p3850) and predefines __gcc_contracts_p4451, by which libstdc++ declares std::contracts::labels::terminating and raises __cpp_lib_contracts; it enables contracts, and with a final --no_contracts it is an error, as is --no_contracts_p4451 with --contracts_p3400 (see optrule/)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++26 --contracts:--c++26 --contracts_p4451:--c++26 --contracts_p3400:--c++26 --contracts_p3850:--c++26 --contracts_p3400 --no_contracts_p4451;fc:--c++26 --contracts_p4451 --no_contracts_p4451 --contracts:--c++23 --contracts_p4451:--c++23 --contracts_p4451 --no_contracts;fc:--c++26 --contracts_p3850 --no_contracts_p3400:--c++26 --no_contracts_p3400 --contracts_p3850:--c++26 --contracts_p3400 --no_contracts_p3850
//match_regex:^(Command-line error: .*|)$
#if TEST_NUMBER == 2 || TEST_NUMBER == 3 || TEST_NUMBER == 4 || \
    TEST_NUMBER == 7 || TEST_NUMBER == 9 || TEST_NUMBER == 10 || \
    TEST_NUMBER == 11
#define P4451 1
#endif
#if TEST_NUMBER == 4 || TEST_NUMBER == 9 || TEST_NUMBER == 10
#define P3097 1
#endif
#if defined(P4451) != defined(__gcc_contracts_p4451)
#error __gcc_contracts_p4451
#endif
#if defined(P4451) && __gcc_contracts_p4451 <= 0
#error the value
#endif
#if !defined(__cpp_contracts)
#error contracts not enabled
#endif
#if defined(P4451) && !(__cpp_contracts > 202502L)
#error __cpp_contracts not raised
#endif
#if (TEST_NUMBER == 1 || TEST_NUMBER == 6) && __cpp_contracts != 202502L
#error __cpp_contracts raised without P4451
#endif
#ifdef __cpp_contracts
#include <contracts>
#if defined(P4451) != (__cpp_lib_contracts > 202502L)
#error __cpp_lib_contracts
#endif
#if defined(P4451) && !defined(P3097) && \
    __cpp_contracts != __cpp_lib_contracts
#error __cpp_contracts and __cpp_lib_contracts differ
#endif
#if defined(P4451) && defined(P3097) && \
    !(__cpp_contracts >= __cpp_lib_contracts)
#error __cpp_contracts below P4451's value
#endif
#ifdef P4451
constexpr auto &t = std::contracts::labels::terminating;
#endif
#endif
