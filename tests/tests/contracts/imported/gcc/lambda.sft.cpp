//remark: imported from gcc:lambda.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
// check that we do not crash when capturing a constified entity in a contract assertion lambda
// { dg-do compile { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe " }
void f(int i, int j) pre( [i, &j](){ return true;} ( ))
{}
