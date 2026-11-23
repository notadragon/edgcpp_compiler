//remark: imported from gcc:contract-repeated-location-constexpr.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
//match_regex: ", line 35: (?:catastrophic )?error
//match_regex: ", line 37: (?:catastrophic )?error
//match_regex: ", line 42: (?:catastrophic )?error
//match_regex: ", line 43: (?:catastrophic )?error
//match_regex: ", line 50: (?:catastrophic )?error
//match_regex: ", line 51: (?:catastrophic )?error
//match_regex: ", line 52: (?:catastrophic )?error
//match_regex: ", line 53: (?:catastrophic )?error
//match_regex: ", line 64: (?:catastrophic )?error
//match_regex: ", line 66: (?:catastrophic )?error
// EDG: adapted -- each violation is diagnosed at the first declaration's
// assertion, not the definition's (EDG-130).
// A contract assertion repeated on a later declaration (the definition) and
// violated in constant evaluation is diagnosed at the definition's
// assertion, never the first declaration's: the shapes of
// contract-repeated-location.C (a non-template function, a member function
// defined outside its class, a member of a class template with a
// non-deduced and with a deduced return type, and a function template;
// preconditions and postconditions).  The location is implementation-defined
// and the assertions are equivalent; the EDG front end reports the first
// declaration's, and Clang the definition's except for a class template
// member with a non-deduced return type (a divergence, GCC-703 in
// open-issues/README.md).
//
// Mirror: tests/tests/contracts/codegen/repeated_contract_location.sft.cpp
// in the edgcpp_compiler fork (run time only); Clang's is
// clang/test/Contracts/contract-repeated-location-constexpr.cpp.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=enforce" }

constexpr int f (const int x) pre (x > 0);	// { dg-error "contract predicate is false in constant expression" }
constexpr int f (const int x) pre (x > 0) { return x; }
constexpr int fp (const int x) post (r : r > x);	// { dg-error "contract predicate is false in constant expression" }
constexpr int fp (const int x) post (r : r > x) { return x; }

struct S
{
  constexpr int m (const int x) const pre (x > 0);	// { dg-error "contract predicate is false in constant expression" }
  constexpr int mp (const int x) const post (r : r > x);	// { dg-error "contract predicate is false in constant expression" }
};
constexpr int S::m (const int x) const pre (x > 0) { return x; }
constexpr int S::mp (const int x) const post (r : r > x) { return x; }

template <class T> struct Z
{
  constexpr T g (const T x) const pre (x > 0);	// { dg-error "contract predicate is false in constant expression" }
  constexpr T gp (const T x) const post (r : r > x);	// { dg-error "contract predicate is false in constant expression" }
  constexpr auto a (const T x) const pre (x > 0);	// { dg-error "contract predicate is false in constant expression" }
  constexpr auto ap (const T x) const post (r : r > x);	// { dg-error "contract predicate is false in constant expression" }
};
template <class T>
constexpr T Z<T>::g (const T x) const pre (x > 0) { return x; }
template <class T>
constexpr T Z<T>::gp (const T x) const post (r : r > x) { return x; }
template <class T>
constexpr auto Z<T>::a (const T x) const pre (x > 0) { return x; }
template <class T>
constexpr auto Z<T>::ap (const T x) const post (r : r > x) { return x; }

template <class T> constexpr T h (const T x) pre (x > 0);	// { dg-error "contract predicate is false in constant expression" }
template <class T> constexpr T h (const T x) pre (x > 0) { return x; }
template <class T> constexpr T hp (const T x) post (r : r > x);	// { dg-error "contract predicate is false in constant expression" }
template <class T> constexpr T hp (const T x) post (r : r > x) { return x; }

// The holding calls are constant (the postconditions here never hold).
static_assert (f (1) == 1);	// OK
static_assert (S{}.m (1) == 1);	// OK
static_assert (Z<int>{}.g (1) == 1);	// OK
static_assert (Z<int>{}.a (1) == 1);	// OK
static_assert (h (1) == 1);	// OK

constexpr int bad_f = f (0);	// Error, at f's first declaration
constexpr int bad_fp = fp (0);	// Error, at fp's first declaration
constexpr int bad_m = S{}.m (0);	// Error, at S::m's first declaration
constexpr int bad_mp = S{}.mp (0);	// Error, at S::mp's first declaration
constexpr int bad_g = Z<int>{}.g (0);	// Error, at Z::g's first declaration
constexpr int bad_gp = Z<int>{}.gp (0);	// Error, at Z::gp's first declaration
constexpr int bad_a = Z<int>{}.a (0);	// Error, at Z::a's first declaration
constexpr int bad_ap = Z<int>{}.ap (0);	// Error, at Z::ap's first declaration
constexpr int bad_h = h (0);	// Error, at h's first declaration
constexpr int bad_hp = hp (0);	// Error, at hp's first declaration
