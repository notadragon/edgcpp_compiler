//remark: imported from clang:generic-lambda-free-function-capture.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-evaluation-semantic=observe %libcxx_flags -o %t && %t

// A generic lambda in a free function's contract captures the parameter or
// result name it names, although the contract is parsed off the declarator,
// before those are reparented onto the function.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/generic-lambda-free-function-capture.C

#include <contracts>

static int handled = 0;
void handle_contract_violation(const std::contracts::contract_violation &) {
  ++handled;
}

int h(int x) pre([&](auto k) { return x + k; }(1) == 6) { return x; }
int f() post(r: [&](auto k) { return r + k; }(1) == 6) { return 5; }

int main() {
  h(5);
  f();
  if (handled != 0)
    __builtin_abort();
}

// REQUIRES: contracts-libcxx, native
