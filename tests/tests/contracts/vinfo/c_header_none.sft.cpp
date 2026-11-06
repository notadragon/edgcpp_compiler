//remark:contracts: under --contract_violation_information=none the P3290 convenience macros of <contracts.h> pass an empty location and comment through edg-gxx, still evaluating their comment argument, as do the P4299 spellings' records; __gcc_contracts_violation_information_none is predefined
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p4299 --contract_evaluation_semantic=observe --contract_violation_information=none
//match_regex:^end$
// Mirrors: gcc/testsuite/gcc.dg/contracts/c-violation-information-none.c
// (gnu_gcc) and clang/test/Contracts/C/c-violation-information-none.c
// (llvm_llvm-project), here in C++ (the suite cannot test C): <contracts.h>
// is our GCC's, and its macros key on the macro the front end predefines.
// c_header_full.sft.cpp is the same program under =full.

#ifndef __gcc_contracts_violation_information_none
#error "__gcc_contracts_violation_information_none is not defined"
#endif

#include <contracts.h>
#include <cstdio>

namespace std { namespace contracts { class contract_violation; } }

static int count;
static int expected_kind;

void handle_contract_violation(const std::contracts::contract_violation &v) {
  const contract_violation_t *cv = (const contract_violation_t *)&v;
  if (stdc_contract_violation_file(cv)[0] != '\0' ||
      stdc_contract_violation_function(cv)[0] != '\0' ||
      stdc_contract_violation_line(cv) != 0 ||
      stdc_contract_violation_column(cv) != 0 ||
      stdc_contract_violation_comment(cv)[0] != '\0' ||
      stdc_contract_violation_kind(cv) != expected_kind ||
      stdc_contract_violation_semantic(cv) != STDC_CONTRACT_OBSERVE)
    __builtin_abort();
  ++count;
}

int pre_f(int x) _Pre(x > 0) { return x; }
int post_f(int x) _Post(r : r > 0) { return x; }
void assert_f(int x) { _ContractAssert(x > 0); }

int main() {
  int evaluated = 0;
  expected_kind = STDC_CONTRACT_PRE;
  pre_f(0);
  expected_kind = STDC_CONTRACT_POST;
  post_f(0);
  expected_kind = STDC_CONTRACT_ASSERT;
  assert_f(0);
  expected_kind = STDC_CONTRACT_MANUAL;
  stdc_handle_observed_contract_violation((++evaluated, "manual comment"));
  if (count != 4 || evaluated != 1) __builtin_abort();
  std::puts("end");
  return 0;
}
