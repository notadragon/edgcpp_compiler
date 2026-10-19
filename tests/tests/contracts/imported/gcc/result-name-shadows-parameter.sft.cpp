//remark: imported from gcc:result-name-shadows-parameter.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=ignore
//match_regex: ", line 20: (?:catastrophic )?error
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 29: (?:catastrophic )?error
// A postcondition result name that conflicts with a parameter, or with a
// lambda's init-capture, is ill-formed ([basic.scope.contract]) whatever the
// evaluation semantic and whether or not the declaration is a definition.
// A simple capture declares nothing, so it does not conflict.
//
// Mirrors: clang/test/Contracts/OpenBugs/result-name-shadows-parameter.cpp
// and clang/test/Contracts/OpenBugs/result-name-shadows-lambda-capture.cpp
// in the llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=ignore" }

int f (int r) post (r: r > 0) { return r; }  // { dg-error "result name shadows a function parameter" }
int g (int r) post (r: r > 0);  // { dg-error "result name shadows a function parameter" }
auto l = [x = 1] (int y) post (x: x > 0) { return y; };  // { dg-error "result name shadows a lambda capture" }
int h (int _) post (_: true);  // OK, name-independent

void k ()
{
  int x = 1;
  auto m = [x] (int y) post (x: x > 0) { return y + x; };  // OK
  auto n = [=] (int r) post (r: r > 0) { return r; };  // { dg-error "result name shadows a function parameter" }
}
