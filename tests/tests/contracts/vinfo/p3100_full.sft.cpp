//remark:contracts: under --contract_violation_information=full, the default, the record of an implicit (P3100) contract assertion that g++'s middle end inserts through edg-gxx (the null dereference check) names the file and line and has a comment
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_sep:!
//options_all:--contracts --contracts_p3100 --contracts_p4298 --contract_evaluation_semantic=observe --contract_group_evaluation_semantic=ub:expr.unary.dereference.nullptr:noexcept_observe
//options:!--contract_violation_information=full
//match_regex:^end$
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/p3100-violation-information-full.C
// (gnu_gcc) and clang/test/Contracts/Runnable/p3100-violation-information-full.cpp
// (llvm_llvm-project); the first case gives no option (the default).
#include <contracts>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std::contracts;

static unsigned expected_line;

void handle_contract_violation(const contract_violation &v) {
  if (std::strcmp(v.location().file_name(), __FILE__) != 0 ||
      !std::strstr(v.location().function_name(), "main") ||
      v.location().line() != expected_line ||
      std::strcmp(v.comment(), "null pointer dereference") != 0 ||
      v.message() != nullptr ||
      v.kind() != assertion_kind::implicit ||
      v.semantic() != evaluation_semantic::noexcept_observe)
    __builtin_abort();
  // The load would fault after an observed violation.
  std::puts("end");
  std::fflush(stdout);
  std::_Exit(0);
}

int main() {
  int *volatile p = nullptr;
  expected_line = __LINE__ + 1;
  volatile int r = *p;
  (void)r;
  __builtin_abort();
}
