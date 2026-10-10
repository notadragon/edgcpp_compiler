//remark: imported from gcc:p3595-dynamic-weak.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-dynamic-weak.json
//options: --c++26 --contracts --contracts_p3400 --contract_configuration_file=p3595-dynamic-weak.json
//use_system_includes: true
//linker_options: -lcontracts
// P3595: with no user selector, the compiler-emitted weak definition (which
// returns the config's compile-time default semantic) drives the semantic.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-dynamic-weak.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
#include <contracts>
using std::contracts::evaluation_semantic;

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

// No user definition of p3595_sel_weak: the weak default returns observe.
void f(int x) pre(x > 0) { }

int main() {
  f(-1);                         // observe -> handler called, continue
  if (violations != 1) __builtin_abort();
  f(1);                          // no violation
  if (violations != 1) __builtin_abort();
}
