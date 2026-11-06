//remark: imported from gcc:handler-in-linkage-spec.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// A replacement ::handle_contract_violation defined inside an
// `extern "C++" { }' block replaces the default handler: the block does not
// change the function's scope.
//
// Mirror: clang/test/Contracts/Runnable/handler-in-linkage-spec.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

static int handled = 0;

extern "C++" {
void handle_contract_violation (const std::contracts::contract_violation&)
{
  ++handled;
}
}

void f (int x) pre (x > 0) {}

int main ()
{
  f (-1);
  if (handled != 1)
    __builtin_abort ();
}
