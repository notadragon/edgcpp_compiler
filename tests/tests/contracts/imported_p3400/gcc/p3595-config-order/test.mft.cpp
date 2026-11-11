//remark: imported from gcc:p3595-config-order.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-config-order-override.json
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_group_evaluation_semantic=safety:observe --contract_configuration_file=p3595-config-order-override.json
//use_system_includes: true
//linker_options: -lcontracts
// P3595: test interleaved command-line ordering.
// The group flag sets safety:observe, then a config file sets safety:ignore.
// Since group flag comes first and first-match wins, safety should be observe.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-group-evaluation-semantic=safety:observe" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-config-order-override.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

void f(int x) pre<"safety"group>(x > 0) { }

int main() {
  // safety group: observe wins (first match from group flag).
  // handler called, execution continues.
  f(-1);
  if (violations != 1) __builtin_abort();
}
