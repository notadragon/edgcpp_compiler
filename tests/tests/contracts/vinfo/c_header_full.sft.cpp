//remark:contracts: under --contract_violation_information=full, and by default, the P3290 convenience macros of <contracts.h> pass the file, function, line and comment through edg-gxx, as do the P4299 spellings' records; __gcc_contracts_violation_information_none is not predefined
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p4299 --contract_evaluation_semantic=observe
//options::--contract_violation_information=full
//match_regex:^end$
// Mirrors: gcc/testsuite/gcc.dg/contracts/c-violation-information-full.c
// (gnu_gcc) and clang/test/Contracts/C/c-violation-information-full.c
// (llvm_llvm-project), here in C++ (the suite cannot test C).  The function
// names and a predicate's comment are g++'s, of the generated C++
// ("(((int)((const int)x)) > 0)"), hence strstr.

#ifdef __gcc_contracts_violation_information_none
#error "__gcc_contracts_violation_information_none is defined"
#endif

#include <contracts.h>
#include <cstdio>
#include <cstring>

namespace std { namespace contracts { class contract_violation; } }

static int count;
static int expected_kind;
static const char *expected_function;
static unsigned expected_line;
static const char *expected_comment;

void handle_contract_violation(const std::contracts::contract_violation &v) {
  const contract_violation_t *cv = (const contract_violation_t *)&v;
  if (std::strcmp(stdc_contract_violation_file(cv), __FILE__) != 0 ||
      !std::strstr(stdc_contract_violation_function(cv), expected_function) ||
      stdc_contract_violation_line(cv) != expected_line ||
      !std::strstr(stdc_contract_violation_comment(cv), expected_comment) ||
      stdc_contract_violation_kind(cv) != expected_kind ||
      stdc_contract_violation_semantic(cv) != STDC_CONTRACT_OBSERVE)
    __builtin_abort();
  ++count;
}

int pre_f(int x) _Pre(x > 0) { return x; }  const unsigned pre_line = __LINE__;
int post_f(int x) _Post(r : r > 0) { return x; }  const unsigned post_line = __LINE__;
void assert_f(int x) { _ContractAssert(x > 0); }  const unsigned assert_line = __LINE__;

static void expect(int kind, const char *function, unsigned line,
                   const char *comment) {
  expected_kind = kind;
  expected_function = function;
  expected_line = line;
  expected_comment = comment;
}

int main() {
  expect(STDC_CONTRACT_PRE, "pre_f", pre_line, "> 0");
  pre_f(0);
  expect(STDC_CONTRACT_POST, "post_f", post_line, "> 0");
  post_f(0);
  expect(STDC_CONTRACT_ASSERT, "assert_f", assert_line, "> 0");
  assert_f(0);
  expect(STDC_CONTRACT_MANUAL, "main", __LINE__ + 1, "manual comment");
  stdc_handle_observed_contract_violation("manual comment");
  if (count != 4) __builtin_abort();
  std::puts("end");
  return 0;
}
