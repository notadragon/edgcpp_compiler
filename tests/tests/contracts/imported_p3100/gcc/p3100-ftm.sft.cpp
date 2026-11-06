//remark: imported from gcc:p3100-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3100
// P3100: gate macro is defined when -fcontracts-p3100 is active.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3100" }

#ifndef __gcc_contracts_p3100
#error "__gcc_contracts_p3100 not defined"
#endif

#if __gcc_contracts_p3100 <= 0
#error "__gcc_contracts_p3100 has wrong value"
#endif
