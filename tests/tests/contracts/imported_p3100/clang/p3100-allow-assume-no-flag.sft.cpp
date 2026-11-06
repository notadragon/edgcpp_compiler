//remark: imported from clang:p3100-allow-assume-no-flag.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3100
// P3100: vendor macro is not defined without -fcontracts-allow-assume.
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3100 -fsyntax-only -verify %s
// expected-no-diagnostics

#ifdef __clang_contracts_allow_assume
#error "__clang_contracts_allow_assume should not be defined without -fcontracts-allow-assume"
#endif
