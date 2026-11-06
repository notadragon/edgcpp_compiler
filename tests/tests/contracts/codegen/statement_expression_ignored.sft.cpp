//remark:contracts: a contract_assert whose predicate is a statement expression, under ignore, keeps its predicate in the lowered IL (EDG-85)
//require:DO_IL_LOWERING 1
//type:fp
//options_all:--g++ --contract_evaluation_semantic=ignore
// The Debug configuration's IL memory-region round trip, which edgy skips,
// stopped at the end of f: run it by hand, or in a C-generating sweep.
int f(int x) {
  contract_assert(({ x > 0; }));
  return x;
}
