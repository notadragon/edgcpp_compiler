//remark: imported from gcc:p3595-caller-dyn-weak.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-caller-dyn-weak.json
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=ignore --contract_configuration_file=p3595-caller-dyn-weak.json
//use_system_includes: true
//linker_options: -lcontracts
// P3595: caller-side dynamic selection -- no user selector, weak default drives it.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-evaluation-semantic=ignore" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-caller-dyn-weak.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
#include <contracts>
using std::contracts::evaluation_semantic;

static int fired = 0;
void handle_contract_violation(const std::contracts::contract_violation&) { ++fired; }

// No definition of p3595_caller_sel_weak: the compiler emits a weak fallback
// returning the config's compile-time default (observe), which drives the
// caller-side check -> handler called, continue.

int f(int x) pre(x > 0) { return x; }   // callee-side ignored (CLI default)
int call() { return f(-1); }            // caller-side dynamic wrapper

int main() {
  call();
  if (fired != 1) __builtin_abort();     // weak default observe caller-side
}
