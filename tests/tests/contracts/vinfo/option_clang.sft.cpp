//remark:contracts: in Clang emulation --contract_violation_information=none predefines Clang's __clang_contracts_violation_information_none (1), not GCC's; =full does not
//require:DO_IL_LOWERING 1
//type:fp
//options:--clang --clang_version=180000 --contract_violation_information=none:--clang --clang_version=180000 --contract_violation_information=full:--clang --clang_version=180000 --contracts --contract_violation_information=none
// Mirror: clang/test/Contracts/contract-violation-information-option.c
// (llvm_llvm-project).
#if TEST_NUMBER != 2
#if __clang_contracts_violation_information_none != 1
#error __clang_contracts_violation_information_none
#endif
#elif defined(__clang_contracts_violation_information_none)
#error __clang_contracts_violation_information_none defined
#endif
#ifdef __gcc_contracts_violation_information_none
#error GCC's macro
#endif
int x;
