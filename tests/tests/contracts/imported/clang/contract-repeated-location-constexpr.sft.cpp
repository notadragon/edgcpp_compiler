//remark: imported from clang:contract-repeated-location-constexpr.cpp
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
//match_regex: ", line 36: (?:catastrophic )?error
//match_regex: ", line 38: (?:catastrophic )?error
//match_regex: ", line 42: (?:catastrophic )?error
//match_regex: ", line 43: (?:catastrophic )?error
//match_regex: ", line 51: (?:catastrophic )?error
//match_regex: ", line 52: (?:catastrophic )?error
//match_regex: ", line 53: (?:catastrophic )?error
//match_regex: ", line 54: (?:catastrophic )?error
//match_regex: ", line 65: (?:catastrophic )?error
//match_regex: ", line 67: (?:catastrophic )?error
// EDG: adapted -- each violation is diagnosed at the first declaration's
// assertion (EDG-130), also for g and gp, and once: Clang reports some at
// the definition's and adds an error and a note on each variable.
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontract-evaluation-semantic=enforce -fsyntax-only -verify %s

// A contract assertion repeated on a later declaration (the definition) and
// violated in constant evaluation is diagnosed at the definition's
// assertion -- except for a member of a class template whose return type is
// not deduced, where it is diagnosed at the in-class declaration's: the
// shapes of contract-repeated-location.cpp (a non-template function, a
// member function defined outside its class, a member of a class template
// with a non-deduced and with a deduced return type, and a function
// template; preconditions and postconditions).  The location is
// implementation-defined and the assertions are equivalent; GCC reports the
// definition's in every case and the EDG front end the first declaration's
// (a divergence, CLANG-684 in open-issues/README.md).
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-repeated-location-constexpr.C
// in the gnu_gcc fork; tests/tests/contracts/codegen/repeated_contract_location.sft.cpp
// in the edgcpp_compiler fork (run time only).

constexpr int f(const int x) pre(x > 0); // expected-error {{contract failed during execution of constexpr function}}
constexpr int f(const int x) pre(x > 0) { return x; }
constexpr int fp(const int x) post(r : r > x); // expected-error {{contract failed during execution of constexpr function}}
constexpr int fp(const int x) post(r : r > x) { return x; }

struct S {
  constexpr int m(const int x) const pre(x > 0); // expected-error {{contract failed during execution of constexpr function}}
  constexpr int mp(const int x) const post(r : r > x); // expected-error {{contract failed during execution of constexpr function}}
};
constexpr int S::m(const int x) const pre(x > 0) { return x; }
constexpr int S::mp(const int x) const post(r : r > x) { return x; }

template <class T> struct Z {
  // A non-deduced return type: Clang too reports the in-class declaration's
  // (CLANG-684).
  constexpr T g(const T x) const pre(x > 0); // expected-error {{contract failed during execution of constexpr function}}
  constexpr T gp(const T x) const post(r : r > x); // expected-error {{contract failed during execution of constexpr function}}
  constexpr auto a(const T x) const pre(x > 0); // expected-error {{contract failed during execution of constexpr function}}
  constexpr auto ap(const T x) const post(r : r > x); // expected-error {{contract failed during execution of constexpr function}}
};
template <class T>
constexpr T Z<T>::g(const T x) const pre(x > 0) { return x; }
template <class T>
constexpr T Z<T>::gp(const T x) const post(r : r > x) { return x; }
template <class T>
constexpr auto Z<T>::a(const T x) const pre(x > 0) { return x; }
template <class T>
constexpr auto Z<T>::ap(const T x) const post(r : r > x) { return x; }

template <class T> constexpr T h(const T x) pre(x > 0); // expected-error {{contract failed during execution of constexpr function}}
template <class T> constexpr T h(const T x) pre(x > 0) { return x; }
template <class T> constexpr T hp(const T x) post(r : r > x); // expected-error {{contract failed during execution of constexpr function}}
template <class T> constexpr T hp(const T x) post(r : r > x) { return x; }

// The holding calls are constant (the postconditions here never hold).
static_assert(f(1) == 1); // OK
static_assert(S{}.m(1) == 1); // OK
static_assert(Z<int>{}.g(1) == 1); // OK
static_assert(Z<int>{}.a(1) == 1); // OK
static_assert(h(1) == 1); // OK

constexpr int bad_f = f(0); // Clang: an error and a note here
constexpr int bad_fp = fp(0); // Clang: an error and a note here
constexpr int bad_m = S{}.m(0); // Clang: an error and a note here
constexpr int bad_mp = S{}.mp(0); // Clang: an error and a note here
constexpr int bad_g = Z<int>{}.g(0); // Clang: an error and a note here
constexpr int bad_gp = Z<int>{}.gp(0); // Clang: an error and a note here
constexpr int bad_a = Z<int>{}.a(0); // Clang: an error and a note here
constexpr int bad_ap = Z<int>{}.ap(0); // Clang: an error and a note here
constexpr int bad_h = h(0); // Clang: an error and a note here
constexpr int bad_hp = hp(0); // Clang: an error and a note here
