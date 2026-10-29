//remark: imported from clang:Runnable/p4298-group.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contracts_p4298 --contract_group_evaluation_semantic=g:noexcept_observe --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// P4298: noexcept_enforce/noexcept_observe are selectable via group config.
// (GCC mirror: g++.dg/contracts/cpp26/p4298-group.C)
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 -fcontracts-p4298 -fcontract-group-evaluation-semantic=g:noexcept_observe -fcontract-evaluation-semantic=ignore %libcxx_flags -o %t
// RUN: %t

#include <contracts>

static int fired = 0;
void handle_contract_violation(const std::contracts::contract_violation& v)
{
  ++fired;
  if (v.semantic() != std::contracts::evaluation_semantic::noexcept_observe)
    __builtin_trap();
}

int f(int x) pre<"g"group>(x > 0) { return x; }

int main()
{
  f(-1);
  if (fired != 1) __builtin_trap();
}
