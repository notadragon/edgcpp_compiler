//remark:contracts: --contract_evaluation_semantic accepted values
//options:--contract_evaluation_semantic=quick_enforce;fp:--contract_evaluation_semantic=bogus;fc:--contract_evaluation_semantic=ignore;fp
void f(int x) {
  contract_assert(x > 0);
}
