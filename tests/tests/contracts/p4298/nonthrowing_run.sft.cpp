//remark:contracts: P4298 through g++: the noexcept semantics report noexcept_observe (6) and noexcept_enforce (7) to the violation handler and terminate when it throws; observe and enforce let the exception out
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p4298
//options:--contract_evaluation_semantic=noexcept_observe -DTHROWS=0:--contract_evaluation_semantic=noexcept_observe -DTHROWS=1:--contract_evaluation_semantic=noexcept_enforce -DTHROWS=1:--contract_evaluation_semantic=observe -DTHROWS=1
//match_regex:^(semantic [2367]|terminate|caught|end)$
#include <contracts>
#include <cstdio>
#include <cstdlib>
#include <exception>

void handle_contract_violation(const std::contracts::contract_violation &v) {
  std::printf("semantic %d\n", (int)v.semantic());
  std::fflush(stdout);
  if (THROWS) throw 1;
}
int f(int x) pre (x > 0) { return x; }
int main() {
  std::set_terminate([] {
    std::puts("terminate");
    std::fflush(stdout);
    std::_Exit(0);
  });
  try { f(0); } catch (int) { std::puts("caught"); }
  std::puts("end");
  return 0;
}
