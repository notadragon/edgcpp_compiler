//remark: imported from gcc:template-decl-contract-lambda-nsscope.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// As template-decl-contract-lambda.C, for a function template at namespace
// scope.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

template <class T> void fa (T t) pre ([&] { return t > 0; } ());
