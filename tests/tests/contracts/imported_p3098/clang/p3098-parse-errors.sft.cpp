//remark: imported from clang:p3098-parse-errors.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
//match_regex: ", line 16: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 25: (?:catastrophic )?error
//match_regex: ", line 28: (?:catastrophic )?error
//match_regex: ", line 32: (?:catastrophic )?error
//match_regex: ", line 34: (?:catastrophic )?error
//match_regex: ", line 39: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3098 -fsyntax-only -verify %s

void f1(int i)
  pre [i] (i > 0); // expected-error {{captures are only allowed on postconditions, not on 'pre'}}

int f2(int& i)
  post [&i] (r: true); // expected-error {{capture-by-reference not allowed in postcondition captures}}

int f3(int& i)
  post [&i = i] (r: true); // expected-error {{capture-by-reference not allowed in postcondition captures}}

int f4(int i)
  post [=] (r: true); // expected-error {{default capture not allowed in postcondition captures}}

int f5(int i)
  post [&] (r: true); // expected-error {{default capture not allowed in postcondition captures}}

struct S {
  int f6()
    post [this] (r: true); // expected-error {{cannot capture 'this' in postcondition captures}}
  int f7()
    post [*this] (r: true); // expected-error {{cannot capture 'this' in postcondition captures}}
};

int global_var = 0;
int f8()
  post [global_var] (r: true); // expected-error {{only function parameters can be captured by copy}}
