//remark:contracts: an init-capture declared outside a contract assertion is const in its predicate
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
//match_regex:", line 13: error
//match_regex:", line 14: error
//match_regex:", line 17: error
// y is declared outside the assertion, so it is const in its predicate, at
// any depth of lambdas, unless a lambda in the assertion captures it by
// copy.
void ok1(int x) {
  contract_assert([y = x]() mutable { return ++y > 0; }());     // OK, in C
}
void c(int x) { [y = x]() mutable { contract_assert(++y > 0); }(); }  // Error
void d(int x) { [&y = x]() { contract_assert(++y > 0); }(); }         // Error
void e(int x) {
  [y = x]() mutable {
    contract_assert([&] { return ++y > 0; }());                 // Error
  }();
}
void ok2(int x) {
  [y = x]() mutable {
    contract_assert([=]() mutable { return ++y > 0; }());       // OK, a copy
  }();
}
