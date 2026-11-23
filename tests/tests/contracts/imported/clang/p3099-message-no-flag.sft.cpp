//remark: imported from clang:p3099-message-no-flag.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 12: (?:catastrophic )?error
//match_regex: ", line 14: (?:catastrophic )?error
//match_regex: ", line 17: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// Without -fcontracts-p3099, the message syntax is rejected.

void f(int x) pre(x > 0, "must be positive") {} // expected-error {{expected ')'}} expected-note {{to match this '('}}

int g(int x) post(r: r >= 0, "non-negative") { return x; } // expected-error {{expected ')'}} expected-note {{to match this '('}}

void h() {
  contract_assert(true, "always true"); // expected-error {{expected ')'}} expected-note {{to match this '('}}
}
