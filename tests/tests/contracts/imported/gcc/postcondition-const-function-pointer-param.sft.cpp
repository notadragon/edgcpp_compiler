//remark: imported from gcc:postcondition-const-function-pointer-param.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// A const function-pointer parameter satisfies the postcondition parameter
// rule (declared const, not of array or function type).
//
// Mirror: clang/test/Contracts/postcondition-const-function-pointer-param.cpp
// in the llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

void f (int (*const g) ()) post (g != nullptr) {}
