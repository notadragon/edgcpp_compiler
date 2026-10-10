extern "C" int puts(const char *);
int f(int x) pre(x > 0) post(r: r > 1) { return x; }
inline void g(int x) { contract_assert(x != 3); }
template <class T> T t(T x) pre(x > 0) { return x; }
struct M { int m(int x) pre(x > 0) { return x; } };
inline auto lam = [](int x) pre(x > 0) { return x; };
