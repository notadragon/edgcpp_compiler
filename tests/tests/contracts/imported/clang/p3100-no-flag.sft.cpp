//remark: imported from clang:p3100-no-flag.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// P3100: gate macro is not defined without -fcontracts-p3100.
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s
// expected-no-diagnostics

#ifdef __clang_contracts_p3100
#error "__clang_contracts_p3100 should not be defined without -fcontracts-p3100"
#endif
