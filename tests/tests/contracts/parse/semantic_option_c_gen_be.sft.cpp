//remark:contracts: the C-generating back end supports only ignore and quick_enforce
//require:DO_IL_LOWERING 1
//type:fc
//options:--contract_evaluation_semantic=observe:--contract_evaluation_semantic=enforce
void f(int x) {
  contract_assert(x > 0);
}
