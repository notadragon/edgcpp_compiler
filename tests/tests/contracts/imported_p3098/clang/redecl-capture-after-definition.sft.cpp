//remark: imported from clang:redecl-capture-after-definition.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3098 -fsyntax-only -verify %s
// expected-no-diagnostics

// A capture-bearing postcondition may be repeated on a redeclaration that
// follows the definition.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/redecl-capture-after-definition.C

int k(const int x) post [c = x] (c > 0) { return x; }
int k(const int x) post [c = x] (c > 0);

int n(const int x) post [c = x] (c > 0);
int n(const int x) post [c = x] (c > 0);
int n(const int x) post [c = x] (c > 0) { return x; }

int m(const int x) post [c = x] (c > 0);
int m(const int y) post [c = y] (c > 0) { return y; }
