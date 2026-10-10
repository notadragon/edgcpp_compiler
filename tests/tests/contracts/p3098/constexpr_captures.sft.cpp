//remark:contracts: P3098 in constant evaluation: captures are initialized with the preconditions and used by the postcondition
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//match_regex:line 22: error: contract predicate is false in constant expression
//match_regex:line 26: error: contract predicate is false in constant expression
// A capture sees the parameter's value when the call starts; the predicate
// sees the result, and may modify its capture (not const).
struct counter {
  int n = 0;
  constexpr int bump() post [old = n] (n == old + 1) { return ++n; }
};
constexpr int twice(int x) post [x] (r: r == 2 * x) {
  int y = x; x = 0; return 2 * y; }
constexpr int ok() {
  counter c;
  c.bump();
  int a = twice(3);  // the capture keeps 3, not the later 0
  return c.n;
}
constexpr int v1 = ok();
constexpr int bad(int x) post [y = x] (r: r == y) { return x + 1; }
constexpr int v2 = bad(1);
constexpr int mut(const int x) post [z = x] (++z == x + 1) { return x; }
constexpr int v3 = mut(5);
constexpr int seen(int x) post [a = x, b = x + 1] (a + 1 == b && a != 7) {
  return x;
}
constexpr int v4 = seen(7);
// Templates; a recursive call has captures of its own.
template<class T> constexpr T tw(T x) post [x] (r: r == 2 * x) {
  T y = x; x = 0; return 2 * y; }
constexpr int rec(int n) post [m = n] (r: r == m) {
  return n == 0 ? 0 : rec(n - 1) + 1; }
constexpr int v5 = tw(4) + rec(3);
// A capture with a nontrivial destructor (EDG-77; see
// constexpr_pack_captures).
struct D { int v; constexpr D(int v) : v(v) {} constexpr ~D() {} };
constexpr int dt(const int x) post [d = D(x)] (d.v == x) { return x; }
constexpr int v6 = dt(1);
int use() { return v1 + v2 + v3 + v4 + v5 + v6; }
