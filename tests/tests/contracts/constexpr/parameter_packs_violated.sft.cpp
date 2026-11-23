//remark:contracts: violated predicates using a function parameter pack's elements, or a parameter after a pack, in constant evaluation
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 14: error: contract predicate is false in constant expression
//match_regex:line 17: error: contract predicate is false in constant expression
//match_regex:line 19: error: contract predicate is false in constant expression
//match_regex:line 22: error: contract predicate is false in constant expression
//match_regex:line 26: error: contract predicate is false in constant expression
// Also runs under the C-generating back end.  The elements of an expanded
// parameter pack share the pack's parameter number (EDG-47): each violation
// is found only if each use reads its own element, and a parameter after
// the pack its own variable.  A violation is reported at its predicate.

template<class... Ts> constexpr int f(Ts... a, int b) pre(b > 0) { return b; }
static_assert(f<int, int>(1, 1, -3) == -3);     // Error
template<class... P> constexpr int w(double t, P... p)
  pre(((p < t) && ...)) { return 0; }
static_assert(w(5.0, 1, 9) == 0);               // Error
template<class... P> constexpr int ix(P... p) pre(p...[1] > 0) { return 0; }
static_assert(ix(1, -1) == 0);                  // Error
template<class... P> constexpr int z(P... p, const int n)
  pre(((p < n) && ...))
  post(r: r == n) { return n; }
static_assert(z<int, int>(1, 4, 3) == 3);       // Error
template<class... P> constexpr int y(P... p, const int n)
  post(r: r == n) { return (p + ...); }
static_assert(y<int, int>(1, 2, 4) == 3);       // Error
