//remark: imported from clang:p3098-redecl.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
//match_regex: ", line 11: (?:catastrophic )?error
//match_regex: ", line 15: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3098 -fsyntax-only -verify %s

// Postcondition with captures vs without captures: different
int g(int x) post [x] (r: r > x); // expected-note {{contract previously specified}}
int g(int x) post (r: r > x); // expected-error {{function redeclaration differs}} expected-note {{in contract specified here}}

// Postcondition with different capture names: different
int h(int x, int y) post [x] (r: r > x); // expected-note {{contract previously specified}}
int h(int x, int y) post [y] (r: r > y); // expected-error {{function redeclaration differs}} expected-note {{in contract specified here}}
