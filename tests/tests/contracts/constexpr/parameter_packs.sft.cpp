//remark:contracts: uses of a function parameter pack's elements, and of a parameter after a pack, in constant evaluation
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// Also runs under the C-generating back end.  The elements of an expanded
// parameter pack share the pack's parameter number (EDG-47): each use must
// still read its own element, and a parameter after the pack its own
// variable.  No predicate here is violated (parameter_packs_violated has
// the violations).

template<class... Ts> constexpr int f(Ts... a, int b) pre(b > 0) { return b; }
static_assert(f<int, int>(-1, -1, 3) == 3);     // OK
template<class... P> constexpr int w(double t, P... p)
  pre(((p < t) && ...)) { return 0; }
static_assert(w(5.0, 1, 2) == 0);               // OK
template<class... P> constexpr int ix(P... p) pre(p...[1] > 0) { return 0; }
static_assert(ix(-1, 1) == 0);                  // OK
template<class... P> constexpr int z(P... p, const int n)
  pre(((p < n) && ...)) post(r: r == n) { return n; }
static_assert(z<int, int>(1, 2, 3) == 3);       // OK
