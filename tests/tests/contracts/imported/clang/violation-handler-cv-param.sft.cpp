//remark: imported from clang:violation-handler-cv-param.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 18: (?:catastrophic )?error
// RUN: %clangxx -std=c++26 -fcontracts %libcxx_flags -fsyntax-only -Xclang -verify %s

// The contract-violation handler's parameter must be exactly "lvalue
// reference to const std::contracts::contract_violation"
// ([basic.contract.handler]), a diagnosable rule, so a reference to const
// volatile is rejected.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/violation-handler-cv-param.C

#include <contracts>

// expected-error@+1 {{handle_contract_violation}}
void handle_contract_violation(const volatile std::contracts::contract_violation &) {}

// REQUIRES: contracts-libcxx
