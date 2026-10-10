//remark: imported from clang:p3595-config-multi-file.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-config-multi-pre.json p3595-config-multi-default.json
//options: --c++26 --contracts --contract_configuration_file=p3595-config-multi-pre.json --contract_configuration_file=p3595-config-multi-default.json
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-configuration-file=%S/p3595-config-multi-pre.json -fcontract-configuration-file=%S/p3595-config-multi-default.json %libcxx_flags -o %t && %t

// P3595: test multiple -fcontract-configuration-file= options.
// First file sets preconditions to ignore.
// Second file sets everything else to observe.
// Since first-match wins, preconditions are ignored and asserts are observed.

#include <contracts>
#include <cstdio>
#include <cstdlib>

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

void f(int x) pre(x > 0) { }

void g(int x) {
  contract_assert(x > 0);
}

int main() {
  // pre with ignore: no handler call
  f(-1);
  if (violations != 0) { printf("FAIL: expected 0, got %d\n", violations); abort(); }

  // contract_assert with observe: handler called, continues
  g(-1);
  if (violations != 1) { printf("FAIL: expected 1, got %d\n", violations); abort(); }

  printf("PASS\n");
}

// REQUIRES: contracts-libcxx, native
