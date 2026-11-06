//remark: imported from clang:contract-violation-user-ctor.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 16: (?:catastrophic )?error
// RUN: %clangxx -std=c++26 -fcontracts %libcxx_flags -fsyntax-only -Xclang -verify %s

// std::contracts::contract_violation has "no user-accessible constructor"
// ([support.contract.violation]), so user code cannot fabricate a violation.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-violation-user-ctor.C

#include <contracts>

void f() {
  std::contracts::contract_violation v(nullptr); // expected-error {{private constructor}}
                                                // expected-note@*:* {{implicitly declared private here}}
}

// REQUIRES: contracts-libcxx
