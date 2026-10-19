//remark: imported from clang:contract-repeated-location.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// EDG: adapted -- a non-template function's violation (f, fp, S::m, S::mp)
// is reported at its first declaration's assertion (EDG-130); a template
// reaches g++ as written, so g++ reports the definition's, also for g and
// gp, where Clang reports the in-class declaration's.  (Type rp, as the
// importer writes a RUN line that compiles and then runs `%t`.)
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-evaluation-semantic=observe %libcxx_flags -o %t && %t
// REQUIRES: contracts-libcxx, native

// A contract assertion repeated on a later declaration (the definition) and
// violated is reported, through contract_violation::location(), at the
// definition's assertion -- except for a member of a class template whose
// return type is not deduced, where it is reported at the in-class
// declaration's: for a non-template function, a member function defined
// outside its class, a member of a class template with a non-deduced and
// with a deduced return type, and a function template; preconditions and
// postconditions.  The location is implementation-defined and the
// assertions are equivalent; GCC reports the definition's in every case and
// the EDG front end the first declaration's, except for a template through
// edg-gxx (a divergence, CLANG-684 in open-issues/README.md).
// contract-repeated-location-constexpr.cpp has the same shapes in constant
// evaluation.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-repeated-location.C
// in the gnu_gcc fork; tests/tests/contracts/codegen/repeated_contract_location.sft.cpp
// in the edgcpp_compiler fork.

#include <contracts>

static unsigned last;
static int count;

void handle_contract_violation(const std::contracts::contract_violation &v) {
  last = v.location().line();
  ++count;
}

// Expect exactly one violation since the last check, at `expected` and not
// at `other`.
static void check(unsigned expected, unsigned other) {
  if (count != 1 || last == other || last != expected)
    __builtin_abort();
  last = 0;
  count = 0;
}

int f(const int x) pre(x > 0); enum { FPRE1 = __LINE__ };
int f(const int x) pre(x > 0) { return x; } enum { FPRE2 = __LINE__ };
int fp(const int x) post(r : r > x); enum { FPOST1 = __LINE__ };
int fp(const int x) post(r : r > x) { return x; } enum { FPOST2 = __LINE__ };

struct S {
  int m(const int x) const pre(x > 0); enum { MPRE1 = __LINE__ };
  int mp(const int x) const post(r : r > x); enum { MPOST1 = __LINE__ };
};
int S::m(const int x) const pre(x > 0) { return x; } enum { MPRE2 = __LINE__ };
int S::mp(const int x) const post(r : r > x) { return x; } enum { MPOST2 = __LINE__ };

template <class T> struct Z {
  T g(const T x) const pre(x > 0); enum { GPRE1 = __LINE__ };
  T gp(const T x) const post(r : r > x); enum { GPOST1 = __LINE__ };
  auto a(const T x) const pre(x > 0); enum { APRE1 = __LINE__ };
  auto ap(const T x) const post(r : r > x); enum { APOST1 = __LINE__ };
};
template <class T>
T Z<T>::g(const T x) const pre(x > 0) { return x; } enum { GPRE2 = __LINE__ };
template <class T>
T Z<T>::gp(const T x) const post(r : r > x) { return x; } enum { GPOST2 = __LINE__ };
template <class T>
auto Z<T>::a(const T x) const pre(x > 0) { return x; } enum { APRE2 = __LINE__ };
template <class T>
auto Z<T>::ap(const T x) const post(r : r > x) { return x; } enum { APOST2 = __LINE__ };

template <class T> T h(const T x) pre(x > 0); enum { HPRE1 = __LINE__ };
template <class T> T h(const T x) pre(x > 0) { return x; } enum { HPRE2 = __LINE__ };
template <class T> T hp(const T x) post(r : r > x); enum { HPOST1 = __LINE__ };
template <class T> T hp(const T x) post(r : r > x) { return x; } enum { HPOST2 = __LINE__ };

int main() {
  f(0);
  check(FPRE1, FPRE2);
  fp(0);
  check(FPOST1, FPOST2);

  S{}.m(0);
  check(S::MPRE1, MPRE2);
  S{}.mp(0);
  check(S::MPOST1, MPOST2);

  // A non-deduced return type: Clang reports the in-class declaration's
  // (CLANG-684); EDG through g++ the definition's.
  Z<int>{}.g(0);
  check(GPRE2, Z<int>::GPRE1);
  Z<int>{}.gp(0);
  check(GPOST2, Z<int>::GPOST1);
  Z<int>{}.a(0);
  check(APRE2, Z<int>::APRE1);
  Z<int>{}.ap(0);
  check(APOST2, Z<int>::APOST1);

  h(0);
  check(HPRE2, HPRE1);
  hp(0);
  check(HPOST2, HPOST1);
}
