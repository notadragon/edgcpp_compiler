//remark:contracts: under --contract_violation_information=none the record of an implicit (P3100) contract assertion that g++'s middle end inserts through edg-gxx (the null dereference check) is empty; kind and semantic are unchanged
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_sep:!
//options_all:--contracts --contracts_p3100 --contracts_p4298 --contract_evaluation_semantic=observe --contract_group_evaluation_semantic=ub:expr.unary.dereference.nullptr:noexcept_observe --contract_violation_information=none
//match_regex:^end$
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/p3100-violation-information-none.C
// (gnu_gcc) and clang/test/Contracts/Runnable/p3100-violation-information-none.cpp
// (llvm_llvm-project).  p3100_full.sft.cpp is the same under =full.
#include <contracts>
#include <cstdio>
#include <cstdlib>

using namespace std::contracts;

void handle_contract_violation(const contract_violation &v) {
  if (v.location().file_name()[0] != '\0' ||
      v.location().function_name()[0] != '\0' ||
      v.location().line() != 0 ||
      v.location().column() != 0 ||
      v.comment()[0] != '\0' ||
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
  volatile int r = *p;
  (void)r;
  __builtin_abort();
}
