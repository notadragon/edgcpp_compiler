//remark: imported from clang:p3098-noncopyable.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
//match_regex: ", line 14: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3098 -fsyntax-only -verify %s

struct NoCopy {
  NoCopy() = default;
  NoCopy(const NoCopy&) = delete; // expected-note {{'NoCopy' has been explicitly marked deleted here}}
};

void f1(NoCopy nc)
  post [nc] (true); // expected-error {{call to deleted constructor}}
