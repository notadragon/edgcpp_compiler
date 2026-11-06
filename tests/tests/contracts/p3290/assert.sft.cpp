//remark:contracts: with P3290, assert from <cassert> reports a violation to the contract violation handler, then aborts
//require:BACK_END_IS_CP_GEN_BE 1
//type:rn
//options_all:--contracts_p3290 -D__STDC_WANT_ASSERT_USES_CONTRACTS__
//match_regex:handler: kind=5
// libstdc++'s <cassert> sees __gcc_contracts_p3290, predefined by the front
// end, and calls __cxa_handle_cassert_violation (assertion_kind::cassert).
#include <cassert>
#include <contracts>
#include <cstdio>

void handle_contract_violation(const std::contracts::contract_violation &v) {
  std::printf("handler: kind=%d\n", static_cast<int>(v.kind()));
  std::fflush(stdout);
}

int main() {
  int x = 1;
  assert(x == 1);
  assert(x == 2);
  std::puts("not reached");
  return 0;
}
