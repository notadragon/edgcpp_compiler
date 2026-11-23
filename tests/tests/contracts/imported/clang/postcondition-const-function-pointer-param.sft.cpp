//remark: imported from clang:postcondition-const-function-pointer-param.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s
// expected-no-diagnostics

// A const function-pointer parameter satisfies the postcondition parameter
// rule: it is declared const, and has neither array nor function type.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/postcondition-const-function-pointer-param.C

void f(int (*const g)()) post(g != nullptr) {}
