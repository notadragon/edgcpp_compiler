//remark: imported from clang:p4283-non-template-recovery.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4283
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 23: (?:catastrophic )?error
//match_regex: ", line 26: (?:catastrophic )?error
//match_regex: ", line 27: (?:catastrophic )?error
//match_regex: ", line 30: (?:catastrophic )?error
//match_regex: ", line 36: (?:catastrophic )?error
//match_regex: ", line 37: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p4283 -fsyntax-only -verify %s

// P4283: a requires clause on a contract assertion of a non-templated
// function is rejected once per clause, and the parser recovers at the
// predicate: the next specifier is still diagnosed and a function body is
// still parsed.  Mirrors GCC's p4283-non-template-recovery.C (GCC-625).

template<typename T> concept C = true;

void f(int x)
  pre requires(true) (x > 0) // expected-error {{requires clause on contract assertion is only allowed on templated functions}}
  post requires(true) (x > 0); // expected-error {{requires clause on contract assertion is only allowed on templated functions}}

void f2(int x)
  pre requires C<int> (x > 0) // expected-error {{requires clause on contract assertion is only allowed on templated functions}}
  pre requires C<int> (x > 1); // expected-error {{requires clause on contract assertion is only allowed on templated functions}}

int f3(int x)
  post requires(true) (r: r > 0) // expected-error {{requires clause on contract assertion is only allowed on templated functions}}
{
  return x;
}

void g(int x) {
  contract_assert requires(true) (x > 0); // expected-error {{requires clause on contract assertion is only allowed on templated functions}}
  contract_assert requires C<int> (x > 0); // expected-error {{requires clause on contract assertion is only allowed on templated functions}}
}

// Control: the declarations after the rejected clauses are still parsed.
void h(int x) pre (x > 0);
int after_f3 = 0;
static_assert(sizeof(after_f3) == sizeof(int));
