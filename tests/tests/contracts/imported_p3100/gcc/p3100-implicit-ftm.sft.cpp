//remark: imported from gcc:p3100-implicit-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3100
// P3100: library FTM is defined when -fcontracts-p3100 is active.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3100" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

#ifndef __cpp_lib_contracts_implicit
#error "__cpp_lib_contracts_implicit not defined"
#endif

#if __cpp_lib_contracts_implicit <= 0
#error "__cpp_lib_contracts_implicit has wrong value"
#endif
