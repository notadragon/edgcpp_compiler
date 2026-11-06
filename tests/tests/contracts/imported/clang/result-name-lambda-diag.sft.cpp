//remark: imported from clang:result-name-lambda-diag.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 16: (?:catastrophic )?error
//match_regex: ", line 18: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// Modifying the result name through a by-reference lambda capture in a
// postcondition gets the contract diagnostic, in a free function's and in a
// member function's.
//
// No GCC mirror: GCC's diagnostic ("increment of read-only reference") has
// no contract-specific form.

int g() post(r: [&] { return ++r > 0; }()) { return 1; } // expected-error {{cannot assign to variable 'r' because it is considered 'const' inside of a contract}}

struct T { int m() post(r: [&] { return ++r > 0; }()); }; // expected-error {{cannot assign to variable 'r' because it is considered 'const' inside of a contract}}
