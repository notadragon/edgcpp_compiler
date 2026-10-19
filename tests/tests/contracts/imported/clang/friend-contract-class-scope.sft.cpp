//remark: imported from clang:friend-contract-class-scope.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s
// expected-no-diagnostics

// A friend declared in a class is in that class's scope, so an unqualified
// name in its contract predicate finds the class's members
// ([basic.lookup.unqual]), whether the friend is defined in the class or
// outside it.
// (GCC mirror: g++.dg/contracts/cpp26/open-bug-friend-contract-class-scope.C,
// xfailed there as GCC-630.)

struct S {
  static constexpr int limit = 10;
  int m(int x) pre(x != limit) { return x; }
  friend int declared(int x) pre(x != limit);
  friend int defined(int x) pre(x != limit) { return x; }
};

int declared(int x) { return x; }
