//remark: imported from clang:Runnable/p4298-json-config.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4298-json-config.json
//options: --c++26 --contracts --contracts_p4298 --contract_evaluation_semantic=ignore --contract_configuration_file=p4298-json-config.json
//use_system_includes: true
//linker_options: -lcontracts
// P4298: noexcept_enforce is selectable via JSON configuration.
// (GCC mirror: g++.dg/contracts/cpp26/p4298-json-config.C)
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p4298 -fcontract-evaluation-semantic=ignore -fcontract-configuration-file=%S/p4298-json-config.json %libcxx_flags -o %t
// RUN: %t

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
  __builtin_trap();
}
