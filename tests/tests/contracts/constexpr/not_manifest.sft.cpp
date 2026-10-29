//remark:contracts: an evaluation that is not manifestly constant-evaluated and meets a violation is not constant
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
// No diagnostic at compile time, and no folding: the violations are
// reported at run time, by the default violation handler (on stderr, where
// the program's own output goes too, to keep them in order).  A variable
// whose initialization checks a precondition is not "never referenced".
#include <cstdio>

constexpr int f(int x) pre(x > 0) { return x; }

struct N { bool r = [](int x) pre(x > 5) { return true; }(1); };

int main() {
  std::fputs("-- local\n", stderr);
  int a = f(-1);
  std::fputs("-- member initializer\n", stderr);
  N n;
  std::fputs("-- end\n", stderr);
  return a == -1 ? 0 : 1;
}
