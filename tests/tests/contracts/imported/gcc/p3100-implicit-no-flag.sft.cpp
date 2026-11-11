//remark: imported from gcc:p3100-implicit-no-flag.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// P3100: library FTM is not defined without -fcontracts-p3100.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

#ifdef __cpp_lib_contracts_implicit
#error "__cpp_lib_contracts_implicit should not be defined without -fcontracts-p3100"
#endif
