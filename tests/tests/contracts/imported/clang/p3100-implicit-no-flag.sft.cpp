//remark: imported from clang:p3100-implicit-no-flag.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// RUN: %clangxx -std=c++26 %s -fcontracts %libcxx_flags -fsyntax-only

// Without -fcontracts-p3100, the library macro is not defined.

#include <contracts>

#ifdef __cpp_lib_contracts_implicit
#error "__cpp_lib_contracts_implicit should not be defined without -fcontracts-p3100"
#endif

// REQUIRES: contracts-libcxx
