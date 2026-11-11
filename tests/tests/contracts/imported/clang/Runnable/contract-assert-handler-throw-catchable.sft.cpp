//remark: imported from clang:Runnable/contract-assert-handler-throw-catchable.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// A throwing violation handler invoked from a contract_assert propagates from
// the assertion-statement, so a try-block enclosing the contract_assert in the
// same (potentially-throwing) function catches it.
// (GCC mirror: g++.dg/contracts/cpp26/basic.contract.eval.p17-3.C.)
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-evaluation-semantic=observe %libcxx_flags -o %t
// RUN: %t

#include <contracts>
#include <cstdlib>

struct E {};

void handle_contract_violation(const std::contracts::contract_violation&) {
  throw E{};
}

int main() {
  try {
    contract_assert(false); // handler throws E -> propagates from this statement
  } catch (E&) {
    std::exit(0);
  }
  __builtin_trap(); // unreachable
}
