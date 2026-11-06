//remark:contracts: P4283 requires-clauses are put out for g++ (templates as written), which discards the same assertions at run time, on every route; with labels, attributes and captures
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p4283 --contracts_p3098 --contracts_p3400 --contract_evaluation_semantic=observe
//match_regex:^violations 12$
#include <concepts>
#include <contracts>
#include <cstdio>

static int n = 0;
void handle_contract_violation(const std::contracts::contract_violation &) {
  ++n;
}
using namespace std::contracts::labels;

template <class T> concept Small = sizeof(T) <= 4;
template <class T> T a(T x) pre requires Small<T> (x > 0) { return x; }
template <class T> T b(T x) pre requires (!Small<T>) (x > 0) { return x; }
template <class T> T c(T x) pre requires Small<T> && std::integral<T> (x > 0)
{ return x; }
template <class T> auto d(T x) post requires Small<T> (r: r > 0) { return x; }
template <class T> T e(T x) {
  contract_assert requires Small<T> (x > 0);
  return x;
}
template <class T> T f(T x) pre requires requires { x + 1; } (x > 0)
{ return x; }
template <class T> T g(T x)
  post <review> requires Small<T> [[maybe_unused]] [c = x] (r: r == c + 1)
{ return x; }
template <class T> struct S {
  void m(T x) pre requires Small<T> (x > 0) {}
  template <class U> void t(U y) pre requires Small<U> (y > 0) {}
};

int main() {
  a(-1); a(-1.0);                       // 1
  b(-1); b(-1.0);                       // 1
  c(-1); c(-1.0f);                      // 1
  d(-1); d(-1.0);                       // 1
  e(-1); e(-1.0);                       // 1
  f(-1);                                // 1
  g(1); g(1.0);                         // 1
  S<int>().m(-1); S<double>().m(-1);    // 1
  S<int>().t(-1); S<int>().t(-1.0);     // 1
  auto gl = [](auto x) pre requires Small<decltype(x)> (x > 0) { return x; };
  gl(-1); gl(-1.0);                     // 1
  auto gl2 = [](auto x) {
    contract_assert requires Small<decltype(x)> (x > 0);
    if (x) contract_assert requires Small<decltype(x)> (x > 0);
    return x;
  };
  gl2(-1); gl2(-1.0);                   // 2
  std::printf("violations %d\n", n);
}
