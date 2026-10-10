//remark: imported from gcc:redecl-capture-after-definition.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=enforce
// A redeclaration after the definition that repeats a capture-bearing
// postcondition is valid (P3098: the captures and the predicate satisfy the
// one-definition rule): it is compared against the capture initializer saved
// when the definition's capture initialization was emitted.
//
// Mirror: clang/test/Contracts/redecl-capture-after-definition.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=enforce" }

int k (const int x) post [c = x] (c > 0) { return x; }
int k (const int x) post [c = x] (c > 0);

int m (const int x) post [x] (x > 0) { return x; }
int m (const int x) post [x] (x > 0);
