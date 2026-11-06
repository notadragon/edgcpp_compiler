//remark:contracts: under --contract_violation_information=none the record of every violation the front end's checks (or g++'s) report has an empty file, function and comment and line and column 0, for pre, post, contract_assert and the P4299 spellings, under both back ends; under =full, and by default, they are set; the last option wins
//options_all:--contracts_p4298 --contracts_p4299 --contract_evaluation_semantic=noexcept_observe
//type:rp
//options::--contract_violation_information=none:--contract_violation_information=full:--contract_violation_information=full --contract_violation_information=none
//match_regex:^(kind|end)
// Mirrors: gcc/testsuite/gcc.dg/contracts/c-violation-information-{none,full}.c
// (gnu_gcc) and clang/test/Contracts/C/c-violation-information-{none,full}.c
// (llvm_llvm-project), here in C++ (the suite cannot test C).
// (No system headers: also runs under the C-generating back end, which
// supports only the noexcept semantics among the checking ones.)  The
// handler reads the violation through libcontracts' C accessors.
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" const char *stdc_contract_violation_comment(const void *);
extern "C" const char *stdc_contract_violation_file(const void *);
extern "C" const char *stdc_contract_violation_function(const void *);
extern "C" unsigned stdc_contract_violation_line(const void *);
extern "C" unsigned stdc_contract_violation_column(const void *);
extern "C" int stdc_contract_violation_kind(const void *);
extern "C" int stdc_contract_violation_semantic(const void *);
namespace std { namespace contracts { class contract_violation; } }

void handle_contract_violation(const std::contracts::contract_violation &v) {
  printf("kind %d semantic %d file [%s] function [%s] line %u column %s "
         "comment [%s]\n",
         stdc_contract_violation_kind(&v),
         stdc_contract_violation_semantic(&v),
         stdc_contract_violation_file(&v),
         stdc_contract_violation_function(&v),
         stdc_contract_violation_line(&v),
         stdc_contract_violation_column(&v) != 0 ? "nonzero" : "0",
         stdc_contract_violation_comment(&v));
  fflush(0);
}

int pre_f(int x) pre(x > 0) { return x; }
int post_f(int x) post(r : r > 0) { return x; }
void assert_f(int x) { contract_assert(x > 0); }
int c_pre_f(int x) _Pre(x > 0) { return x; }
int c_post_f(int x) _Post(r : r > 0) { return x; }
void c_assert_f(int x) { _ContractAssert(x > 0); }

int main() {
  pre_f(0);
  post_f(0);
  assert_f(0);
  c_pre_f(0);
  c_post_f(0);
  c_assert_f(0);
  puts("end");
  return 0;
}
