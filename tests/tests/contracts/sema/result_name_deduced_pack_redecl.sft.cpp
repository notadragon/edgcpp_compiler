//remark:contracts: a fold over a function parameter pack in a postcondition with a result name, on a non-defining declaration of a function template (or member template) with a deduced return type (EDG-108)
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn:-DBAD=2;rn:-DBAD=3;rn
// (No system headers: also runs under the C-generating back end.)  The
// predicate waits for the return type, and is scanned from the first
// declaration's tokens after the definition's body.  From GCC's
// p3098-capture-deduced-return.C (EDG-96).
template <class... T> auto g(const T... t) post(r: ((t > r) && ...));
template <class... T> auto g(const T... t) post(r: ((t > r) && ...)) { return 0; }
template <class C> struct X {
  template <class... T>
  auto m(const T... t) post(r: r == sizeof...(t) && ((t > 0) && ...));
};
template <class C> template <class... T>
auto X<C>::m(const T... t) post(r: r == sizeof...(t) && ((t > 0) && ...)) {
  return (int)sizeof...(t);
}
struct Y {
  template <class... T> auto m(const T... t) post(r: ((t > r) && ...));
};
template <class... T> auto Y::m(const T... t) post(r: ((t > r) && ...)) {
  return 0;
}
int main() {
  int r = g(1, 2) + X<int>().m(1, 2) - 2 + Y().m(3);
  if (BAD == 1) r += g(1, -2);
  if (BAD == 2) r += X<int>().m(-1);
  if (BAD == 3) r += Y().m(1, 0);
  return r;
}
