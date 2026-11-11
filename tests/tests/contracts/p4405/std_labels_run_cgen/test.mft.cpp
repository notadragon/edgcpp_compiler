//remark:contracts: P4405 with the C-generating back end, DECISIONS.md E198 (d): diagnose-then-enforce is the standard diagnose label combined with a label whose compute_semantic returns an enforcing semantic (here quick_enforce, which the C-generating back end checks; it does not check enforce); the combination leaves diagnose allowed, so pre<diagnose | enforced> is warned about at compile time and checked at run time whatever the build configures (here ignore), while pre<diagnose> alone keeps the configured ignore: g(-1) returns, f(-1) traps; as GCC's p4405-std-labels-diagnose-enforced-ignore-run.C
//require:DO_IL_LOWERING 1
//type:ra
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore
//match_regex:^(g returned|f returned)$
//match_regex:line ([0-9]+): (error|warning): (contract .*)
// (No system headers: the C-generating back end cannot find them natively;
// analysis_stub.h stands in for <contracts>.)
#include "analysis_stub.h"
using std::contracts::evaluation_semantic;
extern "C" int puts(const char *);
extern "C" int fflush(void *);

struct enforced_t {
  using assertion_control_object = enforced_t;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return evaluation_semantic::quick_enforce; }
};
constexpr enforced_t enforced{};

void g(int x) pre<diagnose>(x > 0) {}
void f(int x) pre<diagnose | enforced>(x > 0) {}

int main(int argc, char **) {
  g(argc - 2);
  puts("g returned");
  fflush(nullptr);  // The trap below loses buffered output.
  f(argc - 2);
  puts("f returned");
  return 0;
}
