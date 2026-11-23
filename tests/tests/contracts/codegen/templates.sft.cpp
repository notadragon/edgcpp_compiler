//remark:contracts: checks of the contract assertions of templates under quick_enforce
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn:-DVIOLATE=4;rn:-DVIOLATE=5;rn:-DVIOLATE=6;rn:-DVIOLATE=7;rn
// Each instantiation is checked against its own contract assertions:
// function templates (also one declared before it is defined), member
// templates, members of class templates and generic lambdas.  A
// requires-expression in a predicate is substituted, not re-parsed, in an
// instantiation.  Under the C-generating back end the checks are EDG's; the
// C++-generating back end prints a template from its tokens, and leaves all
// contract assertions to g++.  (No system headers: the C-generating
// configuration cannot find them.)
extern "C" int puts(const char *);

template <class T> T tf(T x) pre(x > 0) post(r: r < 50) {
  contract_assert(x != 7);
  return x;
}
template <class T> T td(T x) pre(x > 1);
template <class T> T td(T y) { return y; }
struct A {
  template <class U> U mt(U u) pre(u != 3) { return u; }
  template <class U> U mr(U u) pre(!requires { ++u; }) { return u; }
};
template <class T> struct B {
  T h(T x) pre(x > limit()) { return x; }
  T rq(T x) pre(!requires { ++x; }) { return x; }
  static T limit() { return 0; }
};

int main() {
  auto gl = [](auto y) pre(y != 4) { return y; };
  B<const int> b;
  if (VIOLATE == 0) {
    tf(1); td(2); A().mt(1); A().mr<const int>(1); b.h(1); b.rq(1); gl(1);
  }
  if (VIOLATE == 1) tf(0);
  if (VIOLATE == 2) tf(60);
  if (VIOLATE == 3) tf(7);
  if (VIOLATE == 4) td(1);
  if (VIOLATE == 5) A().mt(3);
  if (VIOLATE == 6) b.h(0);
  if (VIOLATE == 7) gl(4);
  puts("end");
  return 0;
}
