//remark:contracts: the location of a violated contract repeated on a later declaration (EDG-130)
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
// A contract repeated on the definition is reported at its first
// declaration's assertion: the function's contract assertions are the first
// declaration's, and the front end puts out only those (as the C-generating
// back end and constant evaluation do).  A template is put out as written,
// so g++ sees the repetition and reports the definition's, for a deduced
// return type too.  GCC reports the definition's in every case; Clang too,
// except for a member of a class template whose return type is not deduced,
// where it reports the first declaration's (EDG-130, a divergence: the
// location is implementation-defined).  repeated_contract_location_constexpr
// has the same shapes in constant evaluation.
#include <contracts>
#include <cstdio>

static unsigned last;
void handle_contract_violation(const std::contracts::contract_violation& v) {
  last = v.location().line();
}

static void report(const char *what, unsigned first, unsigned definition) {
  std::printf("%s: %s\n", what,
              last == first ? "first declaration"
              : last == definition ? "definition" : "other");
  last = 0;
}

int f(const int x) post(r: r > x); enum { F1 = __LINE__ };
int f(const int x) post(r: r > x) { return x; } enum { F2 = __LINE__ };

struct S {
  int m(const int x) const pre(x > 0); enum { M1 = __LINE__ };
};
int S::m(const int x) const pre(x > 0) { return x; } enum { M2 = __LINE__ };

template <class T> struct Z {
  T g(const T x) const post(r: r > x); enum { G1 = __LINE__ };
  auto a(const T x) const pre(x > 0); enum { A1 = __LINE__ };
};
template <class T>
T Z<T>::g(const T x) const post(r: r > x) { return x; } enum { G2 = __LINE__ };
template <class T>
auto Z<T>::a(const T x) const pre(x > 0) { return x; } enum { A2 = __LINE__ };

template <class T> T h(const T x) pre(x > 0); enum { H1 = __LINE__ };
template <class T> T h(const T x) pre(x > 0) { return x; } enum { H2 = __LINE__ };

int main() {
  f(0);
  report("function", F1, F2);
  S{}.m(0);
  report("member", S::M1, M2);
  Z<int>{}.g(0);
  report("class template member", Z<int>::G1, G2);
  Z<int>{}.a(0);
  report("class template member, deduced return type", Z<int>::A1, A2);
  h(0);
  report("function template", H1, H2);
}
