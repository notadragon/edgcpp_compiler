//remark: imported from clang:p3400-group-config.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=enforce --contract_group_evaluation_semantic=safety:observe!--c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=enforce --contract_group_evaluation_semantic=safety:observe --contract_group_evaluation_semantic=safety.memory:ignore
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 %libcxx_flags -fsyntax-only -fcontract-evaluation-semantic=enforce -fcontract-group-evaluation-semantic=safety:observe
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 %libcxx_flags -fsyntax-only -fcontract-evaluation-semantic=enforce -fcontract-group-evaluation-semantic=safety:observe -fcontract-group-evaluation-semantic=safety.memory:ignore

// Label groups feed into P3595 config resolution.
// Group-specific -fcontract-group-evaluation-semantic flags match labels.

#include <contracts>
using namespace std::contracts;
using namespace std::contracts::labels;

// With -fcontract-group-evaluation-semantic=safety:observe,
// contracts in group "safety" get observe semantic.
void f(int x) pre<"safety"group>(x > 0) {}

// Hierarchical: "safety.memory" also matches the "safety"group config.
void g(int x) pre<"safety.memory"group>(x > 0) {}

// Unrelated group: "performance" falls through to default (enforce).
void h(int x) pre<"performance"group>(x > 0) {}

// No group: falls through to default (enforce).
void i(int x) pre(x > 0) {}

// Combined labels: group concatenation
void j(int x) pre<"safety"group | "performance"group>(x > 0) {}

// REQUIRES: contracts-libcxx
