//remark:contracts: constant evaluation of a postcondition that binds a reference to a parameter or takes its address (EDG-79)
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// The parameters live until the call completes, after the function's body:
// a postcondition may refer to them through a reference or a pointer.
constexpr bool positive(const int &y) { return y > 0; }
constexpr int a(const int x) post(r: *&x == r) { return x; }
constexpr int b(const int x) post(r: positive(x)) { return x; }
constexpr int c(const int x) post(r: [&x, r] { return x == r; }()) { return x; }
constexpr int d(const int x, const int y) post(r: &x != &y) { return x + y; }
struct S {
  int m;
  constexpr S(const int x) post(positive(x)) : m(x) {}
};
static_assert(a(1) == 1);
static_assert(b(1) == 1);
static_assert(c(1) == 1);
static_assert(d(1, 2) == 3);
static_assert(S(1).m == 1);
