//remark: imported from clang:p3099-message-redecl.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3099
//match_regex: ", line 17: (?:catastrophic )?error
//match_regex: ", line 23: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3099 -fsyntax-only -verify %s

// P3099: redeclaration sameness checking for the diagnostic message.
// (GCC mirror: g++.dg/contracts/cpp26/p3099-message-redecl.C)
//
// Message sameness is part of the contract-specifier-sequence comparison and
// uses the extracted message text; without it a mismatched diagnostic message
// across redeclarations goes undiagnosed.

void f(int x) pre(x > 0, "must be positive"); // expected-note {{contract previously specified with a different diagnostic message}}
void f(int x) pre(x > 0, "different message"); // expected-error {{differs in contract specifier sequence}} expected-note {{in contract specified here}}

void g(int x) pre(x > 0, "same message");
void g(int x) pre(x > 0, "same message"); // OK

void h(int x) pre(x > 0, "has message"); // expected-note {{contract previously specified with a different diagnostic message}}
void h(int x) pre(x > 0); // expected-error {{differs in contract specifier sequence}} expected-note {{in contract specified here}}

void i(int x) pre(x > 0);
void i(int x) pre(x > 0); // OK, neither has a message
