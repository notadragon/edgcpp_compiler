//remark:contracts: in GNU mode the valueless vendor macros are 1, as g++ defines them, so #if can test them: __gcc_contracts_violation_information_none (--contract_violation_information=none) and __gcc_contracts_allow_assume (--contracts_allow_assume) (EDG-157)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--contracts_p3100 --contracts_allow_assume --contract_violation_information=none
// Through edg-gxx the front end preprocesses, so it must define them as g++
// does (g++ -dM -E: "#define __gcc_contracts_violation_information_none 1",
// "#define __gcc_contracts_allow_assume 1"); an empty definition made each
// #if below an error ("expected an expression").
#if __gcc_contracts_violation_information_none
#else
#error __gcc_contracts_violation_information_none is false
#endif
#if __gcc_contracts_allow_assume
#else
#error __gcc_contracts_allow_assume is false
#endif
#if __gcc_contracts_violation_information_none != 1 || \
    __gcc_contracts_allow_assume != 1
#error not 1
#endif
int x;
