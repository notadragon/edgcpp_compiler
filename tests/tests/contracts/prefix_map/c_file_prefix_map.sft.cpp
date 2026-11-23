//remark:contracts: through edg-gxx -ffile-prefix-map remaps the file name a violation of a P4299 contract assertion reports (g++ builds the record), as it remaps __FILE__
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p4299 --contract_evaluation_semantic=observe -ffile-prefix-map==FILE-PREFIX
//match_regex:^(file|count) 
// Mirrors: gcc/testsuite/gcc.dg/contracts/c-contract-location-file-prefix-map.c
// (gnu_gcc) and clang/test/Contracts/C/c-contract-location-file-prefix-map.c
// (llvm_llvm-project), here in C++ with the P4299 spellings and <contracts.h> (the
// suite cannot test C).  edg_gxx.sh passes the option to the front end,
// which remaps __FILE__, and to g++, which remaps the violation's file name:
// the two are the same, and begin with the prefix.

#include <contracts.h>
#include <cstdio>
#include <cstring>

namespace std { namespace contracts { class contract_violation; } }

static int count;

void handle_contract_violation(const std::contracts::contract_violation &v) {
  const char *file =
      stdc_contract_violation_file((const contract_violation_t *)&v);
  std::printf("file %s\n", file);
  if (std::strncmp(file, "FILE-PREFIX", 11) != 0 ||
      std::strcmp(file, __FILE__) != 0)
    __builtin_abort();
  ++count;
}

int pre_f(int x) _Pre(x > 0) { return x; }
int post_f(int x) _Post(r : r > 0) { return x; }
void assert_f(int x) { _ContractAssert(x > 0); }

int main() {
  pre_f(0);
  post_f(0);
  assert_f(0);
  std::printf("count %d\n", count);
  if (count != 3) __builtin_abort();
  return 0;
}
