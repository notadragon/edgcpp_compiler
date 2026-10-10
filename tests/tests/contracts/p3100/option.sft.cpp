//remark:contracts: with contracts, --contracts_p3100 (or --contracts_p3850, unless --[no_]contracts_p3100 is given) predefines our GCC's P3100 macro, by which libstdc++ defines __cpp_lib_contracts_implicit, and enables contracts; --contracts_allow_assume (not implied by --contracts_p3850, not enabling contracts) predefines __gcc_contracts_allow_assume
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++26:--c++26 --contracts_p3100:--c++26 --contracts_p3850:--c++26 --contracts_p3850 --no_contracts_p3100:--c++23 --contracts_p3100:--c++23 --contracts_p3100 --no_contracts:--c++26 --contracts_allow_assume:--c++23 --contracts_allow_assume:--c++26 --contracts_p3850 --contracts_allow_assume
// Through edg-gxx, in GNU mode, the macros are our GCC's (libstdc++ tests
// __gcc_contracts_p3100).
#if TEST_NUMBER == 2 || TEST_NUMBER == 3 || TEST_NUMBER == 5 || \
    TEST_NUMBER == 9
#define P3100 1
#endif
#if TEST_NUMBER == 7 || TEST_NUMBER == 9
#define ALLOW_ASSUME 1
#endif
#if TEST_NUMBER != 6 && TEST_NUMBER != 8
#define CONTRACTS 1
#endif
#if defined(CONTRACTS) != defined(__cpp_contracts)
#error __cpp_contracts
#endif
#ifdef P3100
#if __gcc_contracts_p3100 <= 0
#error __gcc_contracts_p3100
#endif
#elif defined(__gcc_contracts_p3100)
#error __gcc_contracts_p3100 defined
#endif
#if defined(ALLOW_ASSUME) != defined(__gcc_contracts_allow_assume)
#error __gcc_contracts_allow_assume
#endif
#ifdef __cpp_contracts
#include <contracts>
#if defined(P3100) != defined(__cpp_lib_contracts_implicit)
#error __cpp_lib_contracts_implicit
#endif
static_assert(int(std::contracts::evaluation_semantic::assume) == 5);
#endif
int x;
