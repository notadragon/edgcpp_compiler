//remark:contracts: a parameter stays alive through every precondition and the body
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// The parameters belong to the function's top-level block, which is created
// after the preconditions; a predicate that allocates storage must not make
// a later predicate, or the body, see them as expired.  Also runs under the
// C-generating back end.
constexpr bool leak(int n) { int *p = new int(n); return *p == n; }
constexpr int b2(int n) pre(leak(3)) pre(n > 0) { return n; }
static_assert(b2(3) == 3);
constexpr int b3(int n) pre(leak(3)) pre(static_cast<const int &>(n) > 0) {
  const int &r = n;
  return r;
}
static_assert(b3(3) == 3);
constexpr int b4(int n) pre(n > 0) { int &r = n; return r; }
static_assert(b4(3) == 3);
struct C {
  int v;
  constexpr C(int a) pre(leak(1)) pre(a > 0) : v(a) {}
};
static_assert(C(2).v == 2);
