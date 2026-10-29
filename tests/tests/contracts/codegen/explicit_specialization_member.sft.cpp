//remark:contracts: checks of the contract assertions of explicit specializations of member templates under quick_enforce
//require:BACK_END_IS_CP_GEN_BE 1
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn
// An explicit specialization of a member template of a class template is
// checked against its own contract assertions, not the member template's
// (which every call below violates); the C++-generating back end puts it
// out from its tokens, for g++ to check.  (Only under edg-gxx: the
// C-generating back end inlines an instance of a member template of a
// class template with a check that names the parameters of the instance,
// which the C compiler rejects, also without an explicit specialization.)
extern "C" int puts(const char *);

template <class T> struct A {
  template <class P> int f(T t, P) pre(t > 100) { return 0; }
};
template <> template <class Q> int A<int>::f(int a, Q q) pre(a > q)
  { return 5; }

int main() {
  if (VIOLATE == 0) A<int>().f(6, 5);
  if (VIOLATE == 1) A<int>().f(6, 6);
  if (VIOLATE == 2) A<int>().f(5, 6);
  puts("end");
  return 0;
}
