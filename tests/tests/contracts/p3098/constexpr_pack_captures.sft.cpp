//remark:contracts: P3098 in constant evaluation: pack captures (EDG-76), and captures whose type has a nontrivial destructor, destroyed once after the postcondition (EDG-77)
//type:fn
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//match_regex:line 15: error: contract predicate is false in constant expression
//match_regex:line 36: error: contract condition is not constant
// Each element is captured when the call starts.
template<class... T> constexpr int sum(T... xs)
  post [xs...] (r: r == (xs + ... + 0)) {
  int r = (xs + ... + 0); ((xs = 0), ...); return r; }
static_assert(sum(1, 2, 3) == 6 && sum() == 0);
template<class... T> constexpr int twice(T... xs)
  post [...ys = xs * 2] (r: r == (ys + ... + 0)) { return 2 * sum(xs...); }
static_assert(twice(1, 2, 3) == 12 && twice() == 0);
template<class... T> constexpr int wrong(T... xs)
  post [...ys = xs] (r: r == (ys + ... + 1)) { return sum(xs...); }
constexpr int w = wrong(1, 2);
// A class capture is destroyed after the postcondition, exactly once.
struct D {
  int *p;
  constexpr D(int *p) : p(p) {}
  constexpr D(const D &o) : p(o.p) {}
  constexpr ~D() { ++*p; }
};
constexpr int made(int *const n) post [d = D(n)] (*d.p == 0) { return 0; }
constexpr int copied(D d) post [e = d] (*e.p == 0) { return 0; }
template<class... T> constexpr int packed(T... ds)
  post [...e = ds] (((*e.p == 0) && ...)) { return 0; }
constexpr int destroyed() { int n = 0; made(&n); return n; }
constexpr int destroyed_copy() { int n = 0; copied(D(&n)); return n; }
constexpr int destroyed_pack() { int n = 0; packed(D(&n), D(&n)); return n; }
static_assert(destroyed() == 1 && destroyed_copy() == 2 &&
              destroyed_pack() == 4);
// A destructor that is not constexpr: the assertion is not constant.
struct R { constexpr R() {} constexpr R(const R &) {} ~R() {} };
R g_r;
constexpr int nc(const R &r) post [s = r] (true) { return 0; }
constexpr int v7 = nc(g_r);
int use() { return w + v7; }
