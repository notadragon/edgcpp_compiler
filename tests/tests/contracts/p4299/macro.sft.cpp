//remark:contracts: with contracts, --contracts_p4299 predefines our GCC's P4299 vendor macro (it does not enable contracts)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++26 --contracts_p4299:--c++26:--c++26 --contracts_p4299 --no_contracts:--c++23 --contracts_p4299:--c++23 --contracts --contracts_p4299
// Through edg-gxx, in GNU mode, the macro is our GCC's.  Its value is the
// implemented revision's date, so only check that it is positive.
#if TEST_NUMBER == 1 || TEST_NUMBER == 5
#define P4299 1
#endif

#ifdef P4299
#if __gcc_contracts_p4299 <= 0
#error __gcc_contracts_p4299
#endif
#elif defined(__gcc_contracts_p4299)
#error __gcc_contracts_p4299 defined
#endif
#ifdef __clang_contracts_p4299
#error __clang_contracts_p4299 defined
#endif
int x;
