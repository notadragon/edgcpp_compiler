//remark:contracts: a fold over a class template's pack in the contract assertions of a member declared in the class and defined outside it is instantiated for each instance (EDG-124)
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn:-DVIOLATE=4;rn:-DVIOLATE=5;rn:-DVIOLATE=6;rn:-DVIOLATE=7;rn:-DVIOLATE=8;rn:-DVIOLATE=9;rn:-DVIOLATE=10;rn:-DVIOLATE=11;rn
// Runs under both back ends: the C-generating one checks quick_enforce
// itself; through edg-gxx the assertions are g++'s.  The specifiers are the
// in-class declaration's, whose pack expansions are recorded in the class's
// template declaration information, not the definition's (EDG-124: "fold
// expression does not refer to a parameter pack").  Each VIOLATE case breaks
// one assertion: a precondition, repeated on the definition or not; a
// postcondition with a deduced return type and a result name; other
// parameter names on the definition; a member template of the class
// template; a member of a nested class template and of a partial
// specialization; a member used before its definition; a static member.
// GCC's and Clang's contract-classtemplate-pack-member-fold are imported in
// imported/.  (No system headers: the C-generating configuration cannot find
// them.)
extern "C" int puts(const char *);

template <class... T> struct Z {
  int f(const T... t) pre(((t > 0) && ...));
  auto g(const T... t) post(r: ((t > r) && ...));
  int h(const T... t) post(r: ((t + r > 0) || ...));
  int k(const T... t) pre(sizeof...(t) > 0);
  int m(const T... t) pre((0 + ... + t) < 10);
  int a(const T... a) pre(((a != 7) && ...));
  template <class U> int b(U u, const T... t) pre(((t > u) && ...));
  template <class V> struct In { int c(V v, const T... t) pre(((t != v) && ...)); };
};
template <class... T>
int Z<T...>::f(const T... t) pre(((t > 0) && ...)) { return 0; }
template <class... T>
auto Z<T...>::g(const T... t) post(r: ((t > r) && ...)) { return 0; }
template <class... T>
int Z<T...>::h(const T... t) post(r: ((t + r > 0) || ...)) { return -10; }
template <class... T>
int Z<T...>::k(const T... t) pre(sizeof...(t) > 0) { return 0; }
template <class... T>
int Z<T...>::m(const T... t) { return 0; }
template <class... T>
int Z<T...>::a(const T... b) { return 0; }
template <class... T> template <class U>
int Z<T...>::b(U u, const T... t) { return 0; }
template <class... T> template <class V>
int Z<T...>::In<V>::c(V v, const T... t) { return 0; }

template <class X, class... T> struct D;
template <class... T> struct D<int, T...> {
  int d(const T... t) pre(((t < 10) && ...));
};
template <class... T> int D<int, T...>::d(const T... t) { return 0; }

template <class... T> struct E { int e(const T... t) pre(((t >= 0) && ...)); };
int use_e(int x) { return E<int, int>().e(1, x); }
template <class... T> int E<T...>::e(const T... t) { return 0; }

template <class... T> struct S {
  static int s(int n, const T... t) pre(((t < n) && ...));
};
template <class... T> int S<T...>::s(int n, const T... t) { return 0; }

int main() {
  Z<int, int> z;
  Z<long, short, int> z3;
  if (VIOLATE == 0) {
    z.f(1, 2); z.g(1, 2); z.h(20, 2); z.k(1, 2); z.m(1, 2); z.a(1, 2);
    z.b(0, 1, 2); Z<int, int>::In<int>().c(3, 1, 2);
    D<int, int, long>().d(1, 2); use_e(0); S<int, int>::s(3, 1, 2);
    z3.f(1, 2, 3); Z<>().m();
  }
  if (VIOLATE == 1) z.f(1, -2);
  if (VIOLATE == 2) z3.g(1, 0, 3);
  if (VIOLATE == 3) z.h(1, 2);
  if (VIOLATE == 4) z.m(5, 6);
  if (VIOLATE == 5) z.a(1, 7);
  if (VIOLATE == 6) z.b(5, 1, 2);
  if (VIOLATE == 7) Z<int, int>::In<int>().c(1, 1, 2);
  if (VIOLATE == 8) D<int, int, long>().d(1, 20);
  if (VIOLATE == 9) use_e(-1);
  if (VIOLATE == 10) S<int, int>::s(2, 1, 2);
  if (VIOLATE == 11) z3.f(1, 2, 0);
  puts("end");
  return 0;
}
