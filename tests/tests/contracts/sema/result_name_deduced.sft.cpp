//remark:contracts: a result name of a function with a deduced return type, on its definition: its type is the deduced one (EDG-9)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 18: error: contract predicate is false in constant expression
//match_regex:line 20: error: contract predicate is false in constant expression
//match_regex:line 23: error: contract predicate is false in constant expression
//match_regex:line 33: error: a result name cannot be introduced in a postcondition of a function returning void
//match_regex:line 35: error: contract predicate is false in constant expression
// The predicate is scanned once the body has deduced the return type, as on
// the declaration: it sees the parameters, not the body's declarations.
constexpr auto f(const int x) post(r: r == x + 1) { return x + 1; }
static_assert(f(1) == 2);
struct S {
  constexpr auto m(const int x) const post(r: r == x) { return x; }
};
static_assert(S{}.m(3) == 3);
constexpr auto g(const int x) post(r: r == x) { return x + 1; }
constexpr int v1 = g(1);
template <class T> constexpr auto t(T x) post(r: r > 0) { return x; }
constexpr int v2 = t(-1);
constexpr int gl = 1;
constexpr auto hidden() post(r: r == gl) { const int gl = 2; return gl; }
constexpr int v3 = hidden();
constexpr auto rec(const int n) post(r: r == n) {
  if (n == 0) return 0;
  return rec(n - 1) + 1;
}
static_assert(rec(3) == 3);
constexpr auto ok = [](int x) post(r: r > 0) { return x; };
static_assert(ok(1) == 1);
decltype(auto) dref(int &x) post(r: &r == &x) { return (x); }
auto v() post(r: true) {}
constexpr int kk = 1;
constexpr auto lam = [](int x) post(r: r == kk) { const int kk = 2; return kk + x; };
constexpr int v4 = lam(0);
int use() { return v1 + v2 + v3 + v4; }
