//remark: imported from gcc:p4299-feature-macro-without.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// Without -fcontracts-p4299, __gcc_contracts_p4299 is not predefined
// (p4299-feature-macro.C).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

#ifdef __gcc_contracts_p4299
#error "__gcc_contracts_p4299"
#endif
