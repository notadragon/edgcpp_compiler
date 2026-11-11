//remark: imported from clang:Contracts/pre-without-parenthesis.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 29: (?:catastrophic )?error
//match_regex: ", line 30: (?:catastrophic )?error
//match_regex: ", line 31: (?:catastrophic )?error
//match_regex: ", line 32: (?:catastrophic )?error
//match_regex: ", line 33: (?:catastrophic )?error
//match_regex: ", line 35: (?:catastrophic )?error
//match_regex: ", line 39: (?:catastrophic )?error
//match_regex: ", line 40: (?:catastrophic )?error
//match_regex: ", line 43: (?:catastrophic )?error
// EDG: adapted -- j comes first: stock EDG's recovery from i's 'garbage' (no
// function body) skips the declaration that follows it.
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// A precondition or postcondition whose predicate is not parenthesized is
// diagnosed once, and parsing recovers (CLANG-638: on a namespace-scope
// declaration the cached contract tokens outlived the abandoned declaration
// and failed an assertion in Declarator::clear, and a member function's
// contract was diagnosed again when its cached tokens were replayed).  So
// are a malformed label, a contract after a well-formed one, a declaration
// abandoned after a well-formed contract, and a local function definition.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/pre-without-parenthesis.C in
// the gnu_gcc fork.  Found by the EDG fork's parse/errors test.

void j(int x) pre<1 x;            // expected-error {{assertion-control labels require '-fcontracts-p3400'}} expected-error {{expected '>'}}
void f(int x) pre x > 0;          // expected-error {{expected '(' after 'pre'}} expected-error {{expected function body after function declarator}}
void g(int x) post x;             // expected-error {{expected '(' after 'post'}} expected-error {{expected function body after function declarator}}
void h(int x) pre(x > 0) post x;  // expected-error {{expected '(' after 'post'}} expected-error {{expected function body after function declarator}}
void i(int x) pre(x > 0) garbage; // expected-error {{expected function body after function declarator}}
void l() {
  void lf(int x) pre(x > 0) {}    // expected-error {{function definition is not allowed here}}
}

struct S {
  void m(int x) pre x > 0;        // expected-error {{expected '(' after 'pre'}} expected-error {{expected ';' at end of declaration list}}
  void n(int x) pre(x > 0) post x; // expected-error {{expected '(' after 'post'}} expected-error {{expected ';' at end of declaration list}}
};

int after = undeclared;           // expected-error {{use of undeclared identifier 'undeclared'}}
