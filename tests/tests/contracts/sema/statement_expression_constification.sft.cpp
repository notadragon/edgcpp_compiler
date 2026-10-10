//remark:contracts: in a statement expression in a predicate, a variable declared outside the assertion is const
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
//match_regex:", line 10: error
//match_regex:", line 11: error
//match_regex:", line 15: error
// The statements of a statement expression in a predicate are in the
// predicate: j and *this are const there, but k, declared in the
// assertion, is not.
void g() { int j = 0; contract_assert(({ ++j; }) > 0); }               // Error
void h() { int j = 0; contract_assert(({ if (j) { ++j; } j; }) > 0); } // Error
void ok() { int j = 0; contract_assert(({ int k = j; ++k; }) > 0); }   // OK
struct S {
  int m;
  void f() { contract_assert(({ ++m; }) > 0); }                       // Error
};
