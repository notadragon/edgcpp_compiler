//remark:contracts: P3099 through edg-gxx: the violation handler gets the message (nullptr without one), re-escaped faithfully
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p3099 --contract_evaluation_semantic=observe
//match_regex:^1 \[x must be positive\]$
//match_regex:^2 \[a "quoted" \\back\tslash\]$
//match_regex:^3 \(null\)$
//match_regex:^4 \[\]$
#include <contracts>
#include <cstdio>

static int n = 0;
void handle_contract_violation(const std::contracts::contract_violation &v) {
  const char *m = v.message();
  if (m == nullptr) std::printf("%d (null)\n", ++n);
  else std::printf("%d [%s]\n", ++n, m);
}

void f(int x) pre(x > 0, "x must be positive") {}
int g(int x) post(r: r > 0, "a \"quoted\" \\back\tslash") { return x; }
void h(int x) pre(x > 0) {}
void k(int x) { contract_assert(x > 0, ""); }

int main() {
  f(0); g(0); h(0); k(0);
  return 0;
}
