//remark:contracts: --contract_violation_information=none predefines __gcc_contracts_violation_information_none (1) whatever the other contract options, as GCC does; =full and the default do not; the last option wins
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--contract_violation_information=none:--c++17 --contract_violation_information=none:--c++26 --no_contracts --contract_violation_information=none:--contract_violation_information=full --contract_violation_information=none:--contract_violation_information=full:--contracts_p3850:--contract_violation_information=none --contract_violation_information=full
// Mirror: clang/test/Contracts/contract-violation-information-option.c
// (llvm_llvm-project), in GNU mode through edg-gxx (the suite cannot test
// C; option_clang.sft.cpp has Clang emulation).
#if TEST_NUMBER <= 4
#define NONE 1
#endif
#if defined(NONE) != defined(__gcc_contracts_violation_information_none)
#error __gcc_contracts_violation_information_none
#endif
#ifdef NONE
#if __gcc_contracts_violation_information_none != 1
#error __gcc_contracts_violation_information_none is not 1
#endif
#endif
#ifdef __clang_contracts_violation_information_none
#error Clang's macro
#endif
int x;
