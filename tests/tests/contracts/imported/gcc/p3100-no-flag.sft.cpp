//remark: imported from gcc:p3100-no-flag.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// P3100: gate macro is not defined without -fcontracts-p3100.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

#ifdef __gcc_contracts_p3100
#error "__gcc_contracts_p3100 should not be defined without -fcontracts-p3100"
#endif
