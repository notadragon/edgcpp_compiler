//remark: imported from gcc:decl-contract-lambda-default-arg.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// A lambda in the contract of a member function declaration, then a default
// argument: parsing the lambda must not leave a placeholder cfun behind (it
// used to, and the default argument crashed on it).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

struct S
{
  void f0 (int x) pre ([] { return true; } ());
};

void f4 (int y = 1);
