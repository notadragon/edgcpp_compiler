//remark:contracts: every evaluation semantic with the C++-generating back end
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--contract_evaluation_semantic=ignore:--contract_evaluation_semantic=observe:--contract_evaluation_semantic=enforce:--contract_evaluation_semantic=quick_enforce
void f(int x) {
  contract_assert(x > 0);
}
