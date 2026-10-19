//remark: imported from gcc:contract-lambda-declaration-gc.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
// A lambda in a contract on a declaration is parsed with no enclosing
// function body: the function context push_function_context saved for it
// must survive the collection the lambda's own finish_function may run.  It
// did not, and a checking build ICEed in pop_function_context (GCC-667);
// the parameters make every collection opportunity collect.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=enforce" }

void f () { [] { return [] { return 1; } (); } (); }

struct S
{
  int m (int y) pre ([=] { return y > 0; } ()) { return y; }
  int n (const int y) post (r: [=] { return r == y; } ());
};

int g (int y) pre ([=] { return [=] { return y > 0; } (); } ());
int h (int y) pre ([=] { return y > 0; } ());
