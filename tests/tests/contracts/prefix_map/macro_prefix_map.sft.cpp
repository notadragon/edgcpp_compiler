//remark:contracts: through edg-gxx -fmacro-prefix-map remaps the file name a contract violation reports (g++ builds the record), as it remaps __FILE__: for pre, post, contract_assert and an implicit (P3100) assertion
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_sep:!
//options_all:--contracts --contract_evaluation_semantic=observe --contracts_p3100 --contract_group_evaluation_semantic=ub:expr.mul.div.by.zero:observe -fmacro-prefix-map==MACRO-PREFIX
//match_regex:^(file|count) 
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/contract-location-macro-prefix-map.C
// (gnu_gcc) and clang/test/Contracts/contract-location-macro-prefix-map.cpp
// (llvm_llvm-project).  file_prefix_map.sft.cpp does the same with
// -ffile-prefix-map.  edg_gxx.sh passes the option to the front end, which
// remaps __FILE__, and to g++, which remaps the violation's file name: the
// two are the same, and begin with the prefix.

#include <contracts>
#include <cstdio>
#include <cstring>

static int count;

void handle_contract_violation(const std::contracts::contract_violation &v) {
  const char *file = v.location().file_name();
  std::printf("file %s\n", file);
  if (std::strncmp(file, "MACRO-PREFIX", 12) != 0 ||
      std::strcmp(file, __FILE__) != 0)
    __builtin_abort();
  ++count;
}

int pre_f(int x) pre(x > 0) { return x; }
int post_f(int x) post(r : r > 0) { return x; }
void assert_f(int x) { contract_assert(x > 0); }
int div_f(int a, int b) { return a / b; }

int main() {
  pre_f(0);
  post_f(0);
  assert_f(0);
  div_f(1, 0);
  std::printf("count %d\n", count);
  if (count != 4) __builtin_abort();
}
