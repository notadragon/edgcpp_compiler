//remark: imported from clang:p3400-label-ftm.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3400 -fsyntax-only -verify %s
// expected-no-diagnostics

#ifndef __cpp_contracts_labels
#error "__cpp_contracts_labels not defined"
#endif

static_assert(__cpp_contracts_labels > 0,
              "__cpp_contracts_labels must be positive");
