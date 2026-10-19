//remark:contracts: the own preconditions and postconditions of a lambda in a template name its captures
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn
// P2900 [expr.prim.lambda.capture]: the predicates of a lambda's function
// contract specifiers name its captures, also when the lambda is in a
// template, where they are scanned in the body of its call operator in the
// template and in each instance.  Runs with both back ends.
extern "C" int puts(const char *);
template <class T> T g(T x) {
  T t = x;
  auto l1 = [=](T u) pre(u > t) { return u + t; };
  auto l2 = [&](const T u) -> T pre(u > t) post(r: r > x) post(u > 0) {
    return u + t + x;
  };
  auto l3 = [c = t](int u) pre(u > c) { return u + c; };
  auto l4 = [=] { return [=](T u) pre(u > t) { return u + x + t; }(t + 1); };
  return l1(2) + l2(2) + l3(2) + l4();
}
template <class T> struct C {
  T k = 1;
  int h(T v) {
    auto l = [=, this](int u) pre(u > v + k) { return u + v + k; };
    return l(5);
  }
};
template <class T> constexpr int k(T lo) {
  auto l = [lo](int b) -> int pre(b > lo) post(r: r > lo) { return b; };
  return l(2);
}
static_assert(k(1) == 2);
int main() {
  if (g(1) != 3 + 4 + 3 + 4) { puts("g"); return 1; }
  C<int> c;
  if (c.h(1) != 7) { puts("C"); return 1; }
#if VIOLATE == 1
  g(5);
#endif
  puts("end");
  return 0;
}
