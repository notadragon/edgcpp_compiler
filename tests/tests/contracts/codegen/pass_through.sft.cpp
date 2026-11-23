//remark:contracts: the C++-generating back end puts contract assertions out as written
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
// g++ checks what cpfe-cp puts out, so each violation below shows that its
// contract assertion reached g++ intact: specifiers on a first declaration
// (and not repeated on the definition, whose parameter has another name), on
// members defined in and out of their class, on a friend and a constructor,
// on lambdas (one written without a parameter list), a postcondition naming
// a result object that cannot be copied, and an indented contract_assert,
// whose comment g++ forms from the predicate, not from this file.
#include <contracts>
#include <cstdio>

void handle_contract_violation(const std::contracts::contract_violation& v) {
  std::printf("kind=%d line=%u comment: %s\n", (int)v.kind(),
              (unsigned)v.location().line(), v.comment());
}

int f(const int x) pre(x > 0) post(r: r > x);
int f(const int y) { return y; }

struct S {
  int v = 1;
  int g(int a) const pre(a < v) { return a; }
  int h(int a) pre(a != 0);
  friend int fr(S &s) pre(s.v > 1) { return s.v; }
  S(int a) pre(a >= 0) : v(a) {}
};
int S::h(int a) { return a; }

struct NC {
  explicit NC(int k) : k(k) {}
  NC(const NC &) = delete;
  int k;
};
NC mk(int i) post(r: r.k == 0) { return NC(i); }

int main() {
  f(0);
  S s(-1);
  s.g(5);
  s.h(0);
  fr(s);
  auto l = [](int q) pre(q > 1) { return q; };
  auto l2 = [] pre(false) { return 0; };
  l(1);
  l2();
  mk(1);
  int x = -1;
          contract_assert(x > 0);
}
