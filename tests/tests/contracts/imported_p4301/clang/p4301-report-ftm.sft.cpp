//remark: imported from clang:p4301-report-ftm.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4301
// P4301: verify the compiler and library feature-test macros for
// contract_violation::report() are defined and have the expected value.
// (GCC mirror: g++.dg/contracts/cpp26/p4301-report-ftm.C)
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p4301 -fsyntax-only %libcxx_flags

#ifndef __cpp_contracts_report
#error "__cpp_contracts_report not defined"
#endif
static_assert(__cpp_contracts_report > 0);

#include <contracts>

#ifndef __cpp_lib_contracts_report
#error "__cpp_lib_contracts_report not defined"
#endif
static_assert(__cpp_lib_contracts_report > 0);

int main() { }

// REQUIRES: contracts-libcxx
