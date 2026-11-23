//remark:contracts: P4451 is part of C++26, whose language mode enables --contracts_p4451 while contracts are on (P4451R2's 202611L for __cpp_contracts, and in GNU mode __gcc_contracts_p4451), unless --[no_]contracts_p4451 is given; before C++26 neither --contracts nor another contracts option enables it, and with --no_contracts it stays off without an error, as in GCC
//type:fp
//options:--c++26:--c++26 --contracts:--c++23 --contracts:--c++23 --contracts_p4298:--c++26 --no_contracts_p4451:--c++26 --no_contracts:--c++23 --contracts_p4451:--c++26 --contract_evaluation_semantic=ignore
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p4451-std-*.C in the gnu_gcc
// fork.
#if TEST_NUMBER == 1 || TEST_NUMBER == 2 || TEST_NUMBER == 7 || \
    TEST_NUMBER == 8
#define P4451 1
#endif
#if TEST_NUMBER == 6
#ifdef __cpp_contracts
#error __cpp_contracts with --no_contracts
#endif
#elif !defined(__cpp_contracts)
#error contracts not enabled
#elif defined(P4451) && __cpp_contracts != 202611L
#error __cpp_contracts not P4451R2's value
#elif !defined(P4451) && __cpp_contracts != 202502L
#error __cpp_contracts raised without P4451
#endif
#if defined(__GNUC__) && defined(P4451) != defined(__gcc_contracts_p4451)
#error __gcc_contracts_p4451
#endif
int pre = 1;                                // OK, not a keyword
