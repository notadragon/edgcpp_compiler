//remark:contracts: a lambda in a contract_assert predicate captures a variable-length array (EDG-110); an array captured by reference there is const
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//match_regex:line 13: error: "a" .* cannot be implicitly captured only for contract assertions
//match_regex:line 28: error: expression must be a modifiable lvalue
//match_regex:line 32: error: expression must be a modifiable lvalue
// A capture by reference in a lambda in a contract predicate is const, as in
// the predicate itself; so is an array (a variable-length one, a GNU
// extension in C++, too).  From GCC's contract-assert-vla-implicit-capture.C
// (EDG-100).
void nested_inside(int n) {
  int a[n];
  auto l = [&] { contract_assert([&] { return a[0] == 0; }()); };  // Error
  l();
}
void explicit_capture(int n) {
  int a[n];
  a[0] = 0;
  contract_assert([&a] { return a[0] == 0; }());                 // OK
}
void implicit_capture(int n) {
  int a[n];
  a[0] = 0;
  contract_assert([&] { return a[0] == 0; }());                  // OK
}
void modify(int n) {
  int a[n];
  contract_assert([&] { return ++a[0] == 0; }());                // Error
}
void modify_fixed() {
  int a[3] = {};
  contract_assert([&] { return ++a[0] == 0; }());                // Error
}
