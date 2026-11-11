//remark: imported from gcc:p3400-facet-empty-combine.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400 --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// P3400: empty_label | empty_label: no compute_semantic, semantic unchanged.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::labels::operator|;

// empty | empty -- no compute_semantic, observe stays observe.
void f(int x)
  pre<(empty_label | empty_label)>(x > 0)
{
}

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

int main() {
  f(-1);  // observe: handler called, continues
  if (violations != 1) __builtin_abort();
  f(1);   // passes
  if (violations != 1) __builtin_abort();
}
