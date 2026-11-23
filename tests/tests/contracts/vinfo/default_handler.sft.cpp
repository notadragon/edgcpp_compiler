//remark:contracts: through edg-gxx libstdc++'s default violation handler prints no empty location part, comment or message under --contract_violation_information=none (pre, a user message, contract_assert, a manual violation with an empty comment), and all of them under =full
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p3099 --contracts_p3290 --contract_evaluation_semantic=observe
//options:--contract_violation_information=none:--contract_violation_information=full
//match_regex:^contract violation
//match_regex:^\[assertion_kind
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/contract-violation-information-default-handler{,-full}.C
// (gnu_gcc) and clang/test/Contracts/Runnable/contract-violation-information-default-handler{,-full}.cpp
// (llvm_llvm-project, which adds the contract_assert and the manual
// violation; libc++'s handler has its own format).
#include <contracts>

void pre_f(int x) pre(x > 0) { }
void msg_f(int x) pre(x > 0, "a user message") { }
void assert_f(int x) { contract_assert(x > 0); }

int main() {
  pre_f(0);
  msg_f(0);
  assert_f(0);
  std::contracts::handle_observed_contract_violation("");
}
