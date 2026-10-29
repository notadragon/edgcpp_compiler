//remark: imported from clang:p3400-label-redecl.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400
//match_regex: ", line 27: (?:catastrophic )?error
//match_regex: ", line 31: (?:catastrophic )?error
//match_regex: ", line 35: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3400 -fsyntax-only -verify %s

// P3400: redeclaration sameness checking for assertion-control labels.
// (GCC mirror: g++.dg/contracts/cpp26/p3400-label-redecl.C)
//
// Label sameness is part of the contract-specifier-sequence comparison;
// without it a mismatched label across redeclarations goes undiagnosed.

struct label_a_t { using assertion_control_object = label_a_t; };
struct label_b_t { using assertion_control_object = label_b_t; };
constexpr label_a_t a{};
constexpr label_b_t b{};

// Same label on redeclaration -- OK.
void same(int x) pre<a>(x > 0);
void same(int x) pre<a>(x > 0) { }

// Different labels -- error.
void diff(int x) pre<a>(x > 0); // expected-note {{contract previously specified with a different assertion-control label}}
void diff(int x) pre<b>(x > 0) { } // expected-error {{differs in contract specifier sequence}} expected-note {{in contract specified here}}

// Label vs no label -- error.
void one_label(int x) pre<a>(x > 0); // expected-note {{contract previously specified with a different assertion-control label}}
void one_label(int x) pre(x > 0) { } // expected-error {{differs in contract specifier sequence}} expected-note {{in contract specified here}}

// No label vs label -- error.
void other_label(int x) pre(x > 0); // expected-note {{contract previously specified with a different assertion-control label}}
void other_label(int x) pre<a>(x > 0) { } // expected-error {{differs in contract specifier sequence}} expected-note {{in contract specified here}}
