//remark: imported from gcc:decl-contract-lambda-default-arg-nsscope.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// As decl-contract-lambda-default-arg.C, at namespace scope.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

void f0 (int x) pre ([x] { return x > 0; } ());

void f4 (int y = 1);
