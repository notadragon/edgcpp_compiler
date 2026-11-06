//remark: imported from clang:p3097-no-flag.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 13: (?:catastrophic )?error
//match_regex: ", line 17: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// Without -fcontracts-p3097, a contract on a virtual function -- a base's, or
// an override's -- is an error.

struct Base {
  virtual void f() pre(true); // expected-error {{contracts on virtual functions require '-fcontracts-p3097'}}
};

struct Child : Base {
  void f() override pre(true); // expected-error {{contracts on virtual functions require '-fcontracts-p3097'}}
};
