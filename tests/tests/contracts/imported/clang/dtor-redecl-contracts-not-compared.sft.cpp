//remark: imported from clang:dtor-redecl-contracts-not-compared.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 18: (?:catastrophic )?error
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 24: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// A non-first declaration of a destructor may not add or change contracts
// ([dcl.contract.func]), defaulted or not, as for a constructor
// (default-outofline-contract.cpp).
//
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/dtor-redecl-contracts-not-compared.C
// and default-outofline-adds-contract.C (the defaulted forms)

struct A { ~A(); };
A::~A() pre(true) = default; // expected-error {{differs in contract specifier sequence}}

struct B { ~B(); };
B::~B() pre(true) {} // expected-error {{differs in contract specifier sequence}}

struct C { ~C() pre(true); };
C::~C() pre(false) {} // expected-error {{differs in contract specifier sequence}}

// Notes are not pinned: their wording is the constructor path's today.
// expected-note-re@* 0+ {{{{.*}}}}
