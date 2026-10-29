//remark: imported from clang:p3100-implicit-ftm.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3100
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3100 %libcxx_flags -fsyntax-only

// Library macro __cpp_lib_contracts_implicit is defined by <contracts>
// when -fcontracts-p3100 is set.

#include <contracts>

#ifndef __cpp_lib_contracts_implicit
#error "__cpp_lib_contracts_implicit not defined after including <contracts>"
#endif

static_assert(__cpp_lib_contracts_implicit > 0,
              "__cpp_lib_contracts_implicit must be positive");

// REQUIRES: contracts-libcxx
