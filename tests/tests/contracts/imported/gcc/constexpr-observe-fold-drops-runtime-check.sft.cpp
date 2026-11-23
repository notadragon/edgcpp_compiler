//remark: imported from gcc:constexpr-observe-fold-drops-runtime-check.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// Under observe, a call whose precondition fails is not folded outside a
// manifestly constant-evaluated context: the check runs, and reports, at run
// time.
//
// Mirror: clang/test/Contracts/constexpr-observe-fold-drops-runtime-check.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

static int n = 0;
void handle_contract_violation (const std::contracts::contract_violation &) { ++n; }

constexpr int f (int x) pre (x > 0) { return x; }
struct P { int v, w; };

int main ()
{
  P p = {f (-1), 2};        // not manifestly constant-evaluated
  int arr[2] = {f (-2), 3}; // likewise
  if (n != 2 || p.v != -1 || arr[0] != -2)
    __builtin_abort ();
}
