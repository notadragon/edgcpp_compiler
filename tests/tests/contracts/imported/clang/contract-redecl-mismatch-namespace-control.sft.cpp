//remark: imported from clang:contract-redecl-mismatch-namespace-control.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 18: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// Control for the CLANG-12 open-bug file
// OpenBugs/deferred-friend-contract-mismatch.cpp, kept out of it because lit
// XFAILs a whole file: the mismatch that two friend declarations get away
// with is diagnosed at namespace scope, which places that gap on the friend
// path rather than on contract matching.

// expected-note@+1 {{contract previously specified with a non-equivalent condition}}
int g(int x) pre(x > 0);
// expected-error@+2 {{function redeclaration differs in contract specifier sequence}}
// expected-note@+1 {{in contract specified here}}
int g(int x) pre(x < 0);
