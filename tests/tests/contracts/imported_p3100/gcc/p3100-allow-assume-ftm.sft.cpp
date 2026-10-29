//remark: imported from gcc:p3100-allow-assume-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3100 --contracts_allow_assume
// P3100: vendor macro is defined when -fcontracts-allow-assume is active.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3100 -fcontracts-allow-assume" }

#ifndef __gcc_contracts_allow_assume
#error "__gcc_contracts_allow_assume not defined"
#endif
