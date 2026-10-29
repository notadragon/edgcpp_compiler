//remark: imported from clang:postcondition-redecl-no-diagnostics.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// RUN:  %clang_cc1 -std=c++26 -fcontracts  -fsyntax-only -verify %s

int f() post(r: r > 0);
int f() post(r: r > 0); // expected-no-diagnostics
