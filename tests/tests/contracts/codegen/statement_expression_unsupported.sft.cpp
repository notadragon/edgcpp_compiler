//remark:contracts: a statement expression in a lambda's precondition is not yet supported by the C-generating back end
//require:DO_IL_LOWERING 1
//type:fn
//options_all:--g++ --contract_evaluation_semantic=quick_enforce
//match_regex:", line 10: error: a statement expression in a precondition or postcondition is not yet supported
// The front end's check copies the predicate of a precondition into the
// body, and cannot copy a statement expression.  A contract_assert's
// predicate is not copied.
int f(int v) {
  auto l = [](int x) pre(({ x > 0; })) { return x; };                // Error
  auto m = [](int x) { contract_assert(({ x > 0; })); return x; };  // OK
  return l(v) + m(v);
}
