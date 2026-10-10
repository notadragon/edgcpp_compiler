//remark: imported from clang:contract-on-deleted-or-defaulted.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 16: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
// RUN: %clang_cc1 -std=c++26 -fcontracts -fsyntax-only -verify %s

// [dcl.contract.func]: a deleted function, and a function defaulted on its
// first declaration, shall not have a function-contract-specifier-seq; both
// are diagnosed.  (The paragraph's virtual-function case is lifted by P3097,
// which this branch implements, so it is not tested here.)
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-on-deleted-or-defaulted.C

void del(int x) pre(x > 0) = delete; // expected-error {{deleted function 'del' cannot have a function-contract-specifier}}

struct S {
  S() pre(true) = default;           // expected-error {{function 'S' defaulted on its first declaration cannot have a function-contract-specifier}}
};

// Control: defaulted on a later declaration; the contract is on the first.
struct T {
  T() pre(true);
};
T::T() = default;
