//remark:contracts: --contracts_p3850 enables contracts, unless --[no_]contracts is given
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++23 --contracts_p3850:--c++23 --contracts_p3850 --no_contracts:--c++23 --contracts_p3850 --no_contracts_p3850:--c++26 --no_contracts_p3850
#if TEST_NUMBER == 1 || TEST_NUMBER == 4
#define CONTRACTS 1
#endif

#if defined(CONTRACTS) != defined(__cpp_contracts)
#error __cpp_contracts
#endif

#ifdef CONTRACTS
int f(int x) pre(x > 0) { contract_assert(x != 2); return x; }
#endif
