//remark: imported from clang:Runnable/p3290-api-quick.cpp
//type: ra
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3290
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3290 %libcxx_flags -o %t
// RUN: not --crash %t

// P3290: handle_quick_enforced_contract_violation terminates without calling the
// handler.  (GCC mirror: p3290-api-quick.C)

#include <contracts>

void handle_contract_violation(const std::contracts::contract_violation&) {
  __builtin_abort();
}

int main() {
  std::contracts::handle_quick_enforced_contract_violation("quick terminate");
  __builtin_abort();
}
