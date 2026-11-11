//remark: imported from clang:p3098-noflag.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 9: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

int f(int i)
  post [i] (r: r >= 0); // expected-error {{postcondition captures require '-fcontracts-p3098'}}
