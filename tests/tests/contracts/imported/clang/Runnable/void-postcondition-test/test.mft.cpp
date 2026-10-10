//remark: imported from clang:Runnable/void-postcondition-test.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-evaluation-semantic=enforce %libcxx_flags -o %t
// RUN: %t

#include "my_assert.h"

int post_count = 0;

bool count_post() {
  ++post_count;
  return true;
}

void implicit_return() post(count_post()) {
}

int main() {
  post_count = 0;
  implicit_return();
  if (!(post_count == 1)) __builtin_abort();
}
