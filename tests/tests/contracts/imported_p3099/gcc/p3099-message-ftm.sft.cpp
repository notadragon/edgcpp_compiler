//remark: imported from gcc:p3099-message-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3099
// P3099: Verify feature-test macros are defined.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3099" }

#ifndef __cpp_contracts_message
#error "__cpp_contracts_message not defined"
#endif

static_assert(__cpp_contracts_message > 0);

#include <contracts>

#ifndef __cpp_lib_contracts_message
#error "__cpp_lib_contracts_message not defined"
#endif

static_assert(__cpp_lib_contracts_message > 0);
