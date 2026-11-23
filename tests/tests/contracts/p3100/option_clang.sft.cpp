//remark:contracts: in Clang emulation the P3100 macros are Clang's
//require:DO_IL_LOWERING 1
//type:fp
//options:--clang --clang_version=180000 --contracts_p3100 --contracts_allow_assume
#if __clang_contracts_p3100 <= 0
#error __clang_contracts_p3100
#endif
#if __clang_contracts_allow_assume != 1
#error __clang_contracts_allow_assume is not 1 (clang defines it as 1)
#endif
#if defined(__gcc_contracts_p3100) || defined(__gcc_contracts_allow_assume)
#error GCC's macros
#endif
int x;
