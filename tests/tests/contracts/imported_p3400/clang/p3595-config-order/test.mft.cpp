//remark: imported from clang:p3595-config-order.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-config-order-override.json
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_group_evaluation_semantic=safety:observe --contract_configuration_file=p3595-config-order-override.json
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 -fcontract-group-evaluation-semantic=safety:observe -fcontract-configuration-file=%S/p3595-config-order-override.json %libcxx_flags -o %t && %t

// P3595: test interleaved command-line ordering.
// The group flag sets safety=observe, then a config file sets safety to ignore.
// Since group flag comes first and first-match wins, safety should be observe.

#include <contracts>
#include <cstdio>
#include <cstdlib>

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

void f(int x) pre<"safety"group>(x > 0) { }

int main() {
  // safety group: observe wins (first match from group flag).
  // handler called, execution continues.
  f(-1);
  if (violations != 1) { printf("FAIL: expected 1, got %d\n", violations); abort(); }

  printf("PASS\n");
}

// REQUIRES: contracts-libcxx, native
