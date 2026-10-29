//remark:contracts: through edg-gxx, postconditions naming the result of functions and lambdas with deduced return types are checked against the deduced result (EDG-9)
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
//match_regex:^violations 5$
#include <contracts>
#include <cstdio>

static int n = 0;
void handle_contract_violation(const std::contracts::contract_violation &) {
  ++n;
}

int gl = 1;
auto f(const int x) post(r: r == x) { return x; }             // OK
auto g(const int x) post(r: r == x) { return x + 1; }         // violation
auto hidden() post(r: r == gl) { int gl = 2; return gl; }     // violation
template <class T> auto t(T x) post(r: r > 0) { return x; }
struct S {
  auto m(const int x) post(r: r == x) { return x; }           // OK
};

int main() {
  f(1);
  g(1);
  hidden();
  t(1);                                                       // OK
  t(-1);                                                      // violation
  S{}.m(2);
  int k = 1;
  auto lam = [k](int x) post(r: r == k) { int k = 5; return k + x; };
  lam(0);                                                     // violation
  auto gen = [](const auto x) post(r: r == x) { return x + 1; };
  gen(1);                                                     // violation
  std::printf("violations %d\n", n);
  return 0;
}
