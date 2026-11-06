//remark: imported from gcc:template-decl-contract-lambda.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// A lambda in the in-class contract of a member template: no statement list
// is open for its closure's DECL_EXPR, and none is needed.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

struct SB
{
  template <class T> void fb (T t) pre ([&] { return t > 0; } ());
};
