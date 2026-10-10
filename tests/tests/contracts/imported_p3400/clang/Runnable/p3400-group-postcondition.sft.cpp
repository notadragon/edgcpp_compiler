//remark: imported from clang:Runnable/p3400-group-postcondition.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_group_evaluation_semantic=safety:observe
//use_system_includes: true
//linker_options: -lcontracts
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 \
// RUN:   -fcontract-group-evaluation-semantic=safety:observe %libcxx_flags -o %t
// RUN: %t

// P3400: identification_label facet with postconditions -- group_names
// extraction works when a postcondition result name is in scope.
// (GCC mirror: g++.dg/contracts/cpp26/p3400-group-postcondition.C)

#include <contracts>

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

int f_post(int x) post<"safety"group>(r: r > 0) { return x; }
void f_pre(int x) pre<"safety"group>(x > 0) { }
void f_post_no_result() post<"safety"group>(true) { }

int main() {
  f_pre(-1);
  if (violations != 1) __builtin_abort();
  f_post(-1);
  if (violations != 2) __builtin_abort();
  f_post(1);
  if (violations != 2) __builtin_abort();
  f_post_no_result();
  if (violations != 2) __builtin_abort();
}
