//remark:contracts: a statement expression in a lambda's precondition or postcondition is checked
//require:BACK_END_IS_CP_GEN_BE 1
//type:rn
//options_all:--contract_evaluation_semantic=enforce
//cases:2
// The checks are made by the compiler of the output (no statement expression
// declares a variable: our GCC cannot yet compile that in a lambda's
// predicate, GCC-632).  The C-generating back end does not yet support them
// (see statement_expression_unsupported).
#include <cstdio>

int f(int v) {
  int lo = 0;
  auto l = [lo](int x) -> int pre(({ x > lo; })) post(r: ({ r > 1; }))
                                                             { return x; };
  auto n = [] pre(({ true; })) { return 0; };
  auto o = [&lo]() pre(({ ({ lo == 0; }); })) { return 0; };
  return l(v) + n() + o();
}

int main() {
  std::fprintf(stderr, "start\n");
  f(2);
#if TEST_NUMBER == 1
  f(0);                        // precondition
#else
  f(1);                        // postcondition
#endif
  std::fprintf(stderr, "not reached\n");
}
