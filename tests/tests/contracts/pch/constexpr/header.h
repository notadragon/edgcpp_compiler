constexpr int cf(int x) pre(x > 0) { return x; }
template <class T> constexpr T ct(T x) pre(x > 0) post(r: r < 9) { return x; }
struct S {
  constexpr S(int x) pre(x > 0) : v(x) {}
  constexpr int mf(int x) const pre(x < v) { contract_assert(x != 2); return x; }
  int v;
};
constexpr auto lam = [](int x) pre(x > 0) { return x; };
int nf(int x) pre(x > 1);
int nf(int x);
