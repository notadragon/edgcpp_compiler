//remark: imported from clang:p3595-unsupported-criterion.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-unsupported-criterion.json
//options: --c++26 --contracts --contract_configuration_file=p3595-unsupported-criterion.json
// RUN: %clangxx -std=c++26 %s -fcontracts -Wno-contract-configuration -fcontract-configuration-file=%S/p3595-unsupported-criterion.json %libcxx_flags -o %t && %t

// P3595 ("File Format"): a configuration entry with an unsupported match
// criterion is skipped, so the entry meant for another module does not turn
// f's precondition off: the violation is enforced and the handler, which
// exits 0, is reached.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p3595-unsupported-criterion.C

#include <contracts>
#include <cstdlib>

void handle_contract_violation(const std::contracts::contract_violation &) {
  std::exit(0); // the precondition was checked
}

int f(int x) pre(x > 0) { return x; }

int main() {
  f(-1);
  return 1; // the entry for another module turned the check off
}

// REQUIRES: contracts-libcxx, native
