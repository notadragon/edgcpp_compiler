//remark: imported from gcc:p4298-json-config.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4298-json-config.json
//options: --c++26 --contracts --contracts_p4298 --contract_evaluation_semantic=ignore --contract_configuration_file=p4298-json-config.json
//use_system_includes: true
//linker_options: -lcontracts
// P4298: noexcept_enforce is selectable via JSON configuration.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4298 -fcontract-evaluation-semantic=ignore" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4298-json-config.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
#include <contracts>
#include <exception>
#include <cstdlib>

void handle_contract_violation(const std::contracts::contract_violation&)
{
  throw 1;
}

int f(int x) pre(x > 0) { return x; }

int main()
{
  std::set_terminate([]() { std::exit(0); });
  f(-1);
  __builtin_abort ();
}
