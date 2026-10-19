//remark: imported from gcc:p4283-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4283
// P4283: Feature-test macro for requires clauses on contracts.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4283" }

#ifndef __cpp_contracts_requires
#error "__cpp_contracts_requires is not defined"
#endif

#if __cpp_contracts_requires <= 0
#error "__cpp_contracts_requires has wrong value"
#endif
