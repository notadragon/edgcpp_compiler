//remark: imported from clang:p4283-ftm.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4283
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p4283 -fsyntax-only -verify %s
// expected-no-diagnostics

#ifndef __cpp_contracts_requires
#error "__cpp_contracts_requires not defined"
#endif

#if __cpp_contracts_requires <= 0
#error "__cpp_contracts_requires has wrong value"
#endif
