//remark:contracts: with contracts, --contracts_p3290 (or --contracts_p3850, unless --[no_]contracts_p3290 is given) predefines our GCC's P3290 macro, and enables contracts
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++23 --contracts_p3850:--contracts_p3850 --no_contracts_p3290:--c++23 --contracts_p3290:--c++23 --contracts_p3290 --no_contracts:--c++26 --contracts_p3290 --no_contracts_p3850:--c++26
// Through edg-gxx, in GNU mode, the macro is our GCC's (libstdc++ tests it).
#if TEST_NUMBER == 1 || TEST_NUMBER == 3 || TEST_NUMBER == 5
#define P3290 1
#endif
#if TEST_NUMBER != 4
#define CONTRACTS 1
#endif

#if defined(CONTRACTS) != defined(__cpp_contracts)
#error __cpp_contracts
#endif
#ifdef P3290
#if __gcc_contracts_p3290 <= 0
#error __gcc_contracts_p3290
#endif
#elif defined(__gcc_contracts_p3290)
#error __gcc_contracts_p3290 defined
#endif
