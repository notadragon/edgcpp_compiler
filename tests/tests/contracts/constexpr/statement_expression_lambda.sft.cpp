//remark:contracts: a statement expression in a lambda's precondition or postcondition in constant evaluation
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:", line 12: error: contract predicate is false in constant expression
//match_regex:", line 17: error: contract predicate is false in constant expression
// A parameter used in a statement expression in a lambda's predicate
// designates the argument, as in the predicate itself, although the
// statement expression is evaluated in a frame of its own.  A violation is
// reported at the assertion.
constexpr int g(int v) {
  auto l = [](const int x) -> int pre(({ x > 0; }))
                                  post(r: ({ r == x + 1; })) { return x + 1; };
  return l(v);
}
constexpr int h(int v) {
  auto l = [](int x) pre(({ ({ x > 0; }); })) { return x; };
  return l(v);
}
static_assert(g(1) == 2);                                           // OK
static_assert(h(1) == 1);                                           // OK
constexpr int z1 = g(0);                                            // Error
constexpr int z2 = h(-1);                                           // Error
