//remark: imported from clang:label-recovery-parens.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --no_contracts_p4451
//match_regex: ", line 19: (?:catastrophic )?error
//match_regex: ", line 20: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fno-contracts-p4451 -fsyntax-only -verify %s

// Without -fcontracts-p3400 (and with P4451, which C++26 implies, turned off)
// an assertion-control label is reported once; a '>' inside parentheses does
// not end the label.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/label-recovery-parens.C

struct L { using assertion_control_object = L; };
constexpr L l{};
constexpr L pick(bool) { return l; }

int f(int x) pre<pick(1 > 0)>(x > 0); // expected-error {{assertion-control labels require '-fcontracts-p3400'}}
int g(int x) pre<l>(x > 0); // expected-error {{assertion-control labels require '-fcontracts-p3400'}}
