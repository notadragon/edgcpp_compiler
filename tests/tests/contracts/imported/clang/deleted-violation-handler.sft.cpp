//remark: imported from clang:deleted-violation-handler.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 17: (?:catastrophic )?error
// RUN: %clangxx -std=c++26 -fcontracts %libcxx_flags -fsyntax-only -Xclang -verify %s

// A deleted ::handle_contract_violation is rejected (treated as if
// [dcl.fct.def.replace] said the handler shall not be deleted), with an error
// naming the handler.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/deleted-violation-handler.C
// (GCC: "'::handle_contract_violation' cannot be deleted").

#include <contracts>

void handle_contract_violation(const std::contracts::contract_violation &) = delete;
// expected-error@-1 {{handle_contract_violation}}

int main() {}

// REQUIRES: contracts-libcxx
