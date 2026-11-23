//remark: imported from gcc:p3595-config-file.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-config-file.json
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=ignore --contract_configuration_file=p3595-config-file.json
//use_system_includes: true
//linker_options: -lcontracts
// P3595: test -fcontract-configuration-file= with a JSON file.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-evaluation-semantic=ignore" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-config-file.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
//
// Config file sets:
//   1. group "safety", callee-side -> observe
//   2. caller-side -> ignore
//   3. everything else -> observe
// The catch-all -fcontract-evaluation-semantic=ignore goes last.
// So effective order: safety(callee):observe, caller:ignore,
// catch-all:observe, then CLI catch-all:ignore (unreachable).

#include <contracts>

static int violations = 0;

void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

// "safety" group -> observe (handler called, continues)
void f_safety(int x) pre<"safety"group>(x > 0) { }

// No group -> observe (from catch-all in config file)
void f_plain(int x) pre(x > 0) { }

int main() {
  // safety group: observe -> handler called, continues
  f_safety(-1);
  if (violations != 1) __builtin_abort();

  // no group: observe -> handler called, continues
  f_plain(-1);
  if (violations != 2) __builtin_abort();

  // Non-violating calls should work fine
  f_safety(1);
  f_plain(1);
  if (violations != 2) __builtin_abort();
}
