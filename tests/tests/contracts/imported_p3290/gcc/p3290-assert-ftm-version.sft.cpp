//remark: imported from gcc:p3290-assert-ftm-version.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3290
// P3290: __cpp_lib_assert_can_use_contracts is defined when <version> is
// included (paper requires it in <version>, <assert.h>, and <cassert>).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3290" }

#include <version>

#ifndef __cpp_lib_assert_can_use_contracts
#error "__cpp_lib_assert_can_use_contracts not defined via <version>"
#endif
