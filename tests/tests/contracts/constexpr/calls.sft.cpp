//remark:contracts: contract assertions of lambdas, templates, constructors and destructors in constant evaluation
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 17: error: contract predicate is false in constant expression
//match_regex:line 20: error: contract predicate is false in constant expression
//match_regex:line 25: error: contract predicate is false in constant expression
//match_regex:line 29: error: contract predicate is false in constant expression
//match_regex:line 39: error: contract predicate is false in constant expression
//match_regex:line 40: error: contract predicate is false in constant expression
//match_regex:line 43: error: contract predicate is false in constant expression
//match_regex:line 44: error: contract predicate is false in constant expression
// Also runs under the C-generating back end.  A predicate's violation is
// reported where its assertion is, here: inner's precondition, not outer's
// (whose predicate calls inner).  Every violation of an evaluation is
// reported, and the evaluation is not cut short by one.

constexpr auto lam = [](int x) pre(x > 0) { return x; };
static_assert(lam(1) == 1);     // OK
static_assert(lam(0) == 0);     // Error
template <class T> constexpr T tmpl(T x) pre(x > 0) { return x; }
static_assert(tmpl(1) == 1);    // OK
static_assert(tmpl(0) == 0);    // Error
template <class T> struct TS {
  T v;
  constexpr T get() const pre(v > 0) { return v; }
};
static_assert(TS<int>{1}.get() == 1);  // OK
static_assert(TS<int>{0}.get() == 0);  // Error
constexpr int inner(int x) pre(x != 3) { return x; }
constexpr int outer(int x) pre(inner(x) > 0) { return x; }
static_assert(outer(1) == 1);   // OK
static_assert(outer(3) == 3);   // Error, at inner's "pre"
// A constructor's precondition and postcondition and a destructor's
// precondition and postcondition: all four are violated.
constexpr bool never = false;
struct D {
  int v;
  constexpr D(const int a)
    pre(a > 0)
    post(v == a + 1)
    : v(a) {}
  constexpr ~D()
    pre(v > 0)
    post(never) {}
};
constexpr int use_d(int a) { D d(a); return d.v; }
static_assert(use_d(0) == 0);   // Error, four times
