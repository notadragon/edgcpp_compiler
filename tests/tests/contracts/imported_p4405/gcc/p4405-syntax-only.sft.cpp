//remark: imported from gcc:p4405-syntax-only.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove
// P4405: prove and diagnose are diagnosed where a check would be emitted,
// so -fsyntax-only, which emits nothing, diagnoses nothing.
//
// Mirror: clang/test/Contracts/p4405-syntax-only.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove -fsyntax-only" }

int f (int x) pre (x > 0) post (r: r > 0)	// { dg-bogus "cannot be proven" }
{
  contract_assert (x != 3);			// { dg-bogus "cannot be proven" }
  return x;
}
