//remark: imported from gcc:redecl-condition-folded.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 23: (?:catastrophic )?error
//match_regex: ", line 24: (?:catastrophic )?error
// The predicates of two declarations' contracts must satisfy the
// one-definition rule, so `x > 1 - 1' and `x > z' do not match `x > 0': the
// conditions are compared as written, not after constant folding.
//
// Mirror: clang/test/Contracts/redecl-condition-folded.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

constexpr int z = 0;
void f (int x) pre (x > 0);
void g (int x) pre (x > 0);
void h (int x) pre (x > 0);

void f (int x) pre (x > 1 - 1) {}	// { dg-error "mismatched contract condition" }
void g (int x) pre (x > z) {}		// { dg-error "mismatched contract condition" }
void h (int x) pre (x > 1) {}		// { dg-error "mismatched contract condition" }
