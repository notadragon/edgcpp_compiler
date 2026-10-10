//remark: imported from clang:Runnable/p4298-normal.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contracts_p4298 --contract_group_evaluation_semantic=obs:noexcept_observe --contract_group_evaluation_semantic=enf:noexcept_enforce --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// P4298: normal (non-terminating) paths for the two new semantics --
// a noexcept_observe violation whose handler does NOT throw continues
// normally (like plain observe), and a noexcept_enforce contract whose
// predicate is NOT violated never calls the handler at all. Uses group
// config (P3400) to select two different semantics for two different
// functions within one translation unit.
//
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 -fcontracts-p4298 -fcontract-group-evaluation-semantic=obs:noexcept_observe -fcontract-group-evaluation-semantic=enf:noexcept_enforce -fcontract-evaluation-semantic=ignore %libcxx_flags -o %t
// RUN: %t

#include <contracts>
#include <cstdlib>

static int fired = 0;

void handle_contract_violation(const std::contracts::contract_violation&)
{
  ++fired;  // does not throw
}

int observe_fn(int x) pre<"obs"group>(x > 0) { return x; }
int enforce_fn(int x) pre<"enf"group>(x > 0) { return x; }

int main()
{
  // noexcept_observe, violated, handler returns normally -> continues.
  observe_fn(-1);
  if (fired != 1)
    __builtin_trap();

  // noexcept_enforce, NOT violated -> handler never called, no termination.
  enforce_fn(1);
  if (fired != 1)
    __builtin_trap();
}
