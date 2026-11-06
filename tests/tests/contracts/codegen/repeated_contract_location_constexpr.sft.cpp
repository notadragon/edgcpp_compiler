//remark:contracts: the location of a contract repeated on a later declaration, violated in constant evaluation (EDG-130)
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex: ", line 25: error: contract predicate is false in constant expression
//match_regex: ", line 29: error: contract predicate is false in constant expression
//match_regex: ", line 34: error: contract predicate is false in constant expression
//match_regex: ", line 35: error: contract predicate is false in constant expression
//match_regex: ", line 42: error: contract predicate is false in constant expression
// A contract repeated on the definition and violated in constant evaluation
// is diagnosed at its first declaration's assertion: the function's
// contract assertions are the first declaration's.  The shapes of
// repeated_contract_location: a function, a member defined outside its
// class, a member of a class template with a non-deduced and with a deduced
// return type, and a function template.  GCC diagnoses the definition's in
// every case; Clang too, except for a member of a class template whose
// return type is not deduced (EDG-130, a divergence: the location is
// implementation-defined).  Through edg-gxx the front end's diagnostic is
// the only one: g++ never sees the program.  The semantic is quick_enforce,
// which both configurations support.  Mirror:
// g++.dg/contracts/cpp26/contract-repeated-location-constexpr.C in the
// gnu_gcc fork, clang/test/Contracts/contract-repeated-location-constexpr.cpp
// in the llvm_llvm-project fork (both imported, adapted, in
// imported/{gcc,clang}).
//
constexpr int f(const int x) post(r: r > x);
constexpr int f(const int x) post(r: r > x) { return x; }

struct S {
  constexpr int m(const int x) const pre(x > 0);
};
constexpr int S::m(const int x) const pre(x > 0) { return x; }

template <class T> struct Z {
  constexpr T g(const T x) const post(r: r > x);
  constexpr auto a(const T x) const pre(x > 0);
};
template <class T>
constexpr T Z<T>::g(const T x) const post(r: r > x) { return x; }
template <class T>
constexpr auto Z<T>::a(const T x) const pre(x > 0) { return x; }

template <class T> constexpr T h(const T x) pre(x > 0);
template <class T> constexpr T h(const T x) pre(x > 0) { return x; }

static_assert(S{}.m(1) == 1);
static_assert(Z<int>{}.a(1) == 1);
static_assert(h(1) == 1);

constexpr int bad_f = f(0);
constexpr int bad_m = S{}.m(0);
constexpr int bad_g = Z<int>{}.g(0);
constexpr int bad_a = Z<int>{}.a(0);
constexpr int bad_h = h(0);
