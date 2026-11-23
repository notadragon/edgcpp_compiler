//remark: imported from gcc:evaluation-semantic-last-wins.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// A repeated -fcontract-evaluation-semantic= is accepted and the last one
// wins, as for any other option: enforce, then observe, observes.
//
// Mirror: clang/test/Contracts/evaluation-semantic-last-wins.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=enforce -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

int violations;
void handle_contract_violation (const std::contracts::contract_violation &)
{
  ++violations;
}

int f (int x) pre (x > 0) { return x; }

int main ()
{
  f (-1);
  if (violations != 1)
    __builtin_abort ();
}
