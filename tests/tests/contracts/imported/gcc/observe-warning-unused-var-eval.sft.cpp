//remark: imported from gcc:observe-warning-unused-var-eval.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
// Under observe, a speculative evaluation that is not manifestly constant
// reports no contract violation.
//
// Mirror: clang/test/Contracts/observe-warning-unused-var-eval.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe" }

struct N { bool r = [] (int x) pre (x > 5) { return true; } (1); };

void use ()
{
  N n;
}
