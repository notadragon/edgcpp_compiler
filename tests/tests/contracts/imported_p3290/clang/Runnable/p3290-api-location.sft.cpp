//remark: imported from clang:Runnable/p3290-api-location.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3290
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3290 %libcxx_flags -o %t
// RUN: %t

// P3290: source_location is populated (default = call site; explicit is honored).
// (GCC mirror: p3290-api-location.C)

#include <contracts>
#include <cstdio>
#include <source_location>

static unsigned last_line = 0;
void handle_contract_violation(const std::contracts::contract_violation& v) {
  last_line = v.location().line();
}

int main() {
  unsigned expected_line = __LINE__ + 1;
  std::contracts::handle_observed_contract_violation("here");
  if (last_line != expected_line) __builtin_abort();

  auto loc = std::source_location::current();
  std::contracts::handle_observed_contract_violation("there", loc);
  if (last_line != loc.line()) __builtin_abort();
  std::printf("PASS\n");
}
