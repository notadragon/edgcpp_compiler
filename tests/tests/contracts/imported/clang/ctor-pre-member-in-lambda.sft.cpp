//remark: imported from clang:Contracts/ctor-pre-member-in-lambda.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 19: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// The implicit (*this). transformation of a member named inside a lambda in
// a constructor precondition still occurs in the predicate
// ([expr.prim.id.general]), so it is ill-formed.  The member is recovered as
// an erroneous expression of its type, so the lambda's return type is still
// deduced as bool and nothing follows from the one error.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/ctor-pre-member-in-lambda.C

struct S {
  int m;
  S(unsigned) pre([&] { return m > 0; }()); // expected-error {{cannot access non-static member 'm' in the precondition of a constructor}}
  S(long) pre([this] { return this->m > 0; }());
  S(char) pre([] { struct L { int q; int get() { return q; } }; return true; }());
  S(int) pre(m > 0); // expected-error {{cannot access non-static member 'm' in the precondition of a constructor}}
};
