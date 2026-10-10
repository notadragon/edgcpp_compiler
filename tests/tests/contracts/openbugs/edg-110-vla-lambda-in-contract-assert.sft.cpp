//remark:contracts: EDG-110: a lambda in a contract_assert predicate that captures a VLA is an internal error (find_vla_dimension)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//cases:2
// The recording is the current, wrong behavior: when this test deviates,
// the bug may be fixed.  Correct: the [&] lambda of nested_inside is
// diagnosed, the lambda in the predicate being the only use of a ("cannot
// be implicitly captured only for contract assertions"), and
// explicit_capture is accepted; both are the internal error instead.
// From GCC's contract-assert-vla-implicit-capture.C (EDG-100).
#if TEST_NUMBER == 1
void nested_inside(int n) {
  int a[n];
  auto l = [&] { contract_assert([&] { return a[0] == 0; }()); };  // Error
  l();
}
#else
void explicit_capture(int n) {
  int a[n];
  contract_assert([&a] { return a[0] == 0; }());                 // OK
}
#endif
