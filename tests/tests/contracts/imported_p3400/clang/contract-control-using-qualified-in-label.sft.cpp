//remark: imported from clang:contract-control-using-qualified-in-label.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3400 -fsyntax-only -verify %s
// expected-no-diagnostics

// Inside an assertion-control scope, qualified lookup considers a `using
// contract_control namespace' directive, so pre<lib::l> finds lab::l; outside
// one it must not (OpenBugs/contract-control-qualified-lookup.cpp, CLANG-505).
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-control-using-qualified.C

namespace lab { struct L { using assertion_control_object = L; }; constexpr L l{}; }
namespace lib { using contract_control namespace lab; }

void f1(int x) pre<lib::l>(x > 0);
auto v = [](int x) pre<lib::l>(x > 0) { return x; };
void f2(int x) { contract_assert<lib::l>(x > 0); }
