//remark:contracts: constant evaluation diagnoses violated preconditions and postconditions whose lambdas capture the function's parameters, this and the result name
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
struct S {
  int m;
  constexpr int f(int x) const pre([this] { return m > 0; }()) {
    return m + x;
  }
  constexpr int k(int x) const pre([this](auto j) { return m > j; }(x)) {
    return m;
  }
};
constexpr int f(int x) pre([&] { return x > 0; }()) { return x; }
constexpr int g(int x) pre([x] { return x > 0; }()) { return x; }
constexpr int g4(int x) pre([&](auto k) { return x > k; }(0)) { return x; }
constexpr int a() post(r: [r] { return r > 0; }()) { return 0; }
constexpr int b() post(r: [&r] { return r > 0; }()) { return 0; }
constexpr int d(const int x) post(r: [=] { return r > x; }()) { return x; }
constexpr int e(int x, int y) pre([&] { return x < y; }()) { return y - x; }
static_assert(S{0}.f(1) == 1);   // Error
static_assert(S{1}.k(1) == 1);   // Error
static_assert(f(0) == 0);        // Error
static_assert(g(0) == 0);        // Error
static_assert(g4(0) == 0);       // Error
static_assert(a() == 0);         // Error
static_assert(b() == 0);         // Error
static_assert(d(1) == 1);        // Error
static_assert(e(3, 1) == -2);    // Error
