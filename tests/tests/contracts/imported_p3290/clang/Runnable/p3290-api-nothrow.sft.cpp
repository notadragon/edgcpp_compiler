//remark: imported from clang:Runnable/p3290-api-nothrow.cpp
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3290
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3290 %libcxx_flags -o %t
// RUN: not --crash %t

// P3290: the nothrow overload terminates if the handler throws.
// (GCC mirror: p3290-api-nothrow.C)

#include <contracts>
#include <new>

void handle_contract_violation(const std::contracts::contract_violation&) {
  throw 42;
}

int main() {
  std::contracts::handle_observed_contract_violation(std::nothrow, "throws");
  __builtin_abort();
}
