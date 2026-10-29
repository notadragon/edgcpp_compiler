//remark: imported from clang:p4283-errors.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4283
//match_regex: ", line 13: (?:catastrophic )?error
//match_regex: ", line 17: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p4283 -fsyntax-only -verify %s

// P4283: Error cases.

// requires clause on non-templated function is ill-formed.
void f(int x)
  pre requires(true) (x > 0); // expected-error {{requires clause on contract assertion is only allowed on templated functions}}

// requires clause on non-template contract_assert.
void g(int x) {
  contract_assert requires(true) (x > 0); // expected-error {{requires clause on contract assertion is only allowed on templated functions}}
}
