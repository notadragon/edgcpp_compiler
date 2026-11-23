//remark:contracts: in Clang emulation the P4299 vendor macro is Clang's
//require:DO_IL_LOWERING 1
//type:fp
//options:--clang --clang_version=180000 --contracts --contracts_p4299
#if __clang_contracts_p4299 <= 0
#error __clang_contracts_p4299
#endif
#ifdef __gcc_contracts_p4299
#error GCC's macro
#endif
int x;
