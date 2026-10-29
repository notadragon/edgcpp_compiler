//remark: imported from gcc:p3400-label-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
// P3400: Feature-test macros.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#ifndef __cpp_contracts_labels
#error "__cpp_contracts_labels not defined"
#endif

#if __cpp_contracts_labels <= 0
#error "__cpp_contracts_labels has wrong value"
#endif

#include <contracts>

#ifndef __cpp_lib_contracts_labels
#error "__cpp_lib_contracts_labels not defined"
#endif
