//remark:contracts: a function's name is in scope in its contract assertions
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
constexpr int rec(int x) pre(x == 0 || rec(x - 1) >= 0) { return x; }
static_assert(rec(2) == 2);
int rec2(const int x) pre(x >= 0) post(r: r == rec2(x)) { return x; }
template <class T>
constexpr T trec(T x) pre(x == 0 || trec(x - 1) >= 0) { return x; }
static_assert(trec(2) == 2);
template <class T> struct Mutual {
  bool f(T v) const pre(g(v)) { return true; }
  bool g(T v) const pre(f(v)) { return true; }
};
bool use_mutual() { Mutual<int> m; return m.f(1); }
class A { friend int fr(A &a) pre(a.p() >= 0); int p() const; };
int fr(A &a) pre(a.p() >= 0) { return a.p(); }      // OK, a friend
