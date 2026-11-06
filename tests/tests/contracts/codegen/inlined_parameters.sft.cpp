//remark:contracts: a call inlined with its contract check uses the inlined copies of the parameters
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn:-DVIOLATE=4;rn:-DVIOLATE=5;rn
// Under the C-generating back end, a member of a class template (or a member
// template of one) is inlined into its caller with its checks, which are
// copies of the predicates naming the instance's parameter variables.  A
// parameter named only by a predicate must be remapped like any other.
// (Was EDG-48: the inlined check named the callee's parameters, so the
// generated C did not compile.)  (No system headers: the C-generating
// configuration cannot find them.)
extern "C" int puts(const char *);

template <class T> struct A {
  T v;
  template <class P> int f(T x, P p) pre(x > p) { return x; }
  template <class P> int g(T x, const P p) post(r: r > p) { return x; }
  int h(T a, T) pre(a > 0) { return 7; }
  bool m(T w) const pre(w >= 0 && n(w)) { return true; }
  bool n(T w) const pre(w < 10) { return true; }
};

int main() {
  A<int> a{1};
  if (VIOLATE == 0) {
    if (a.f(6, 5) != 6 || a.g(6, 5) != 6 || a.h(1, 0) != 7 || !a.m(3)) {
      return 1;
    }  /* if */
  }  /* if */
  if (VIOLATE == 1) a.f(5, 6);
  if (VIOLATE == 2) a.g(5, 6);
  if (VIOLATE == 3) a.h(0, 1);
  if (VIOLATE == 4) a.m(-1);
  if (VIOLATE == 5) a.m(10);
  puts("end");
  return 0;
}
