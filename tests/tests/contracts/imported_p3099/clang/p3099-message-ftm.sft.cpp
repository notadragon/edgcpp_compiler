//remark: imported from clang:p3099-message-ftm.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3099
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3099 -fsyntax-only %libcxx_flags

// P3099: feature-test macros for user-defined diagnostic messages are defined.
// (GCC mirror: g++.dg/contracts/cpp26/p3099-message-ftm.C)

#ifndef __cpp_contracts_message
#error "__cpp_contracts_message not defined"
#endif
static_assert(__cpp_contracts_message > 0);

#include <contracts>

#ifndef __cpp_lib_contracts_message
#error "__cpp_lib_contracts_message not defined"
#endif
static_assert(__cpp_lib_contracts_message > 0);

int main() { }

// REQUIRES: contracts-libcxx
