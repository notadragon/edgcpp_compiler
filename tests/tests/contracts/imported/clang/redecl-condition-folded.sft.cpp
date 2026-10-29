//remark: imported from clang:redecl-condition-folded.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 18: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// The predicates of two declarations' contracts must satisfy the
// one-definition rule: `x > 1 - 1' and `x > z' do not match `x > 0'.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/redecl-condition-folded.C

constexpr int z = 0;
void f(int x) pre(x > 0); // expected-note {{contract previously specified with a non-equivalent condition}}
void g(int x) pre(x > 0); // expected-note {{contract previously specified with a non-equivalent condition}}

void f(int x) pre(x > 1 - 1) {} // expected-error {{function redeclaration differs in contract specifier sequence}} expected-note {{in contract specified here}}
void g(int x) pre(x > z) {}     // expected-error {{function redeclaration differs in contract specifier sequence}} expected-note {{in contract specified here}}
