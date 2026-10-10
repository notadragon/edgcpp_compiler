//remark: imported from clang:Runnable/p3400-group-basic.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_group_evaluation_semantic=safety:observe
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 \
// RUN:   -fcontract-group-evaluation-semantic=safety:observe %libcxx_flags -o %t
// RUN: %t

// P3400: identification_label facet -- basic group labels and the
// -fcontract-group-evaluation-semantic flag.
// (GCC mirror: g++.dg/contracts/cpp26/p3400-group-basic.C)

#include <contracts>

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

// "safety" group -- configured to observe.
void f_safety(int x) pre<"safety"group>(x > 0) { }
// "other" group -- no config, falls through to default (enforce).
void f_other(int x) pre<"other"group>(x > 0) { }
// No group -- falls through to default (enforce).
void f_plain(int x) pre(x > 0) { }

int main() {
  f_safety(-1);
  if (violations != 1) __builtin_abort();

  f_safety(1);
  f_other(1);
  f_plain(1);
  if (violations != 1) __builtin_abort();
}
