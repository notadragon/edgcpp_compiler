//remark:contracts: P4405 through edg-gxx: an ignore contract_assert whose predicate calls a function is a premise for g++'s proof shadow but leaves the real code unchanged, so a function that is only declared links
//type:rp
//require:BACK_END_IS_CP_GEN_BE 1
//options:--c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore -fno-contracts-prove-assume -O2
//match_regex:^(f 5)$
// g++'s shadow assumes the ignore evaluations' predicates (which call
// opaque, never defined, and positive, an inline function never inlined)
// and proves the prove evaluation after them, conditionally; the region is
// gone from the real function before anything is emitted, so the program
// links without a definition of opaque, and runs.
// Mirror: g++.dg/contracts/cpp26/p4405-own-pre-call-premise-real.C in the
// gnu_gcc fork (which inspects g++'s dumps and assembly).
#include <contracts>
using namespace std::contracts::labels;
extern "C" int printf(const char *, ...);

bool opaque(int);  // never defined
[[gnu::noinline]] inline bool positive(int x) { return x > 0; }
int sink(int x) { return x; }

int f(int x)
{
  contract_assert(opaque(x) && x > 1);
  contract_assert(positive(x));
  contract_assert<prove>(x > 1);  // OK
  return sink(x);
}

int main() {
  printf("f %d\n", f(5));
  return 0;
}
