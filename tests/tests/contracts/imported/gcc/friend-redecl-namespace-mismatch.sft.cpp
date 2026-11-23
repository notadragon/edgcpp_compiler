//remark: imported from gcc:friend-redecl-namespace-mismatch.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
//match_regex: ", line 23: (?:catastrophic )?error
// Watch test for an OPEN bug (GCC-27, deferred): a friend declaration that
// redeclares an ordinary namespace-scope function with a different contract
// is not diagnosed unless a later declaration happens to flush the
// redeclaration-match queue; nothing after this class does.  Two friend
// declarations of one function are diagnosed:
// contract-friend-deferred-mismatch.C.
//
// Mirror: clang/test/Contracts/qualified-friend-member-contract-mismatch.cpp
// in the llvm_llvm-project fork (its struct A), which Clang passes
// (CLANG-639).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=enforce" }

int h (int x) pre (x > 0);

struct D
{
  friend int h (int x) pre (x < 0); // { dg-error "mismatched contract condition" }
};

int h (int x) { return x; }
