//remark: imported from clang:Runnable/postcondition-result-name-mangling.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// A postcondition with a result name does not create a mangling conflict with an
// identically named overload that has an extra parameter. (GCC mirror:
// g++.dg/contracts/cpp26/name_mangling.C.)
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-evaluation-semantic=observe %libcxx_flags -o %t
// RUN: %t

#include <contracts>

int f() post(r : r > 1) { return 2; }
void f(int) post(true) {}

int main() {
  f();
  f(2);
  return 0;
}
