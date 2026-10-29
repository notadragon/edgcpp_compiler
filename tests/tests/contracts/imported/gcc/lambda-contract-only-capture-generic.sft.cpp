//remark: imported from gcc:lambda-contract-only-capture-generic.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 18: (?:catastrophic )?error
// A generic lambda whose precondition names a variable captured only for it:
// diagnosed, and the instantiation that follows does not crash on the
// capture the pattern had and the instantiation does not.
// bail-out line is the excess output).
//
// Mirror: clang/test/Contracts/lambda-contract-only-capture.cpp in
// the llvm_llvm-project fork, which passes.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

void h (int t)
{
  auto t1 = [=] (auto u) pre (u > t) {};	// { dg-error "not implicitly captured" }
  t1 (1);
}
