//remark: imported from clang:Runnable/violation-template-function-name.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// The violation location's function_name() identifies the specific template
// instantiation, including its template arguments. (GCC mirror:
// g++.dg/contracts/cpp26/contracts-tmpl-spec2.C, which checks that the violation
// output names the instantiation.)
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-evaluation-semantic=observe %libcxx_flags -o %t
// RUN: %t

#include <contracts>
#include "my_assert.h"
#include <cstring>

using namespace std::contracts;

const char* exp_needle;
unsigned calls = 0;

void handle_contract_violation(const contract_violation& v) {
  ++calls;
  if (!(__builtin_strstr(v.location().function_name(), exp_needle) != nullptr)) __builtin_abort();
}

template <class T> T f(T x) pre(x > 0) { return x; }

int main() {
  exp_needle = "T = int";
  f<int>(-1);
  exp_needle = "T = double";
  f<double>(-1.0);
  if (!(calls == 2)) __builtin_abort();
  return 0;
}
