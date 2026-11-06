//remark: imported from clang:p3400-control-header.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=observe
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 %libcxx_flags -fcontract-evaluation-semantic=observe -fsyntax-only

// After #include <contracts>, empty_label is available unqualified
// in assertion-control expressions via the header's
// using contract_control namespace directive.

#include <contracts>

void f(int x) pre<empty_label>(x > 0) {}

int g(int x) post<empty_label>(r: r >= 0) { return x; }

void h() {
  contract_assert<empty_label>(true);
}

// REQUIRES: contracts-libcxx
