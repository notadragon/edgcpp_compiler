//remark:contracts: a predicate's side effects do not persist in constant evaluation
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// Also runs under the C-generating back end.  The predicate's value counts,
// but whatever it modifies is restored once the value is known.
constexpr bool bump(int *p) { ++*p; return true; }

// [basic.contract.eval] Example 1: the bound is 1 whatever the semantic.
constexpr int f(int i) {
  contract_assert((++const_cast<int &>(i), true));
  return i;
}
static_assert(f(1) == 1);

// Modification of the caller's object, by a precondition reached twice.
constexpr int pre_side(int *p) pre(bump(p)) { return *p; }
constexpr int run_pre() {
  int x = 0;
  int a = pre_side(&x);
  int b = pre_side(&x);
  return a * 10 + b + x;
}
static_assert(run_pre() == 0);

// A class object, a member, and an array element.
struct Counter { int hits; int a[2]; };
constexpr bool touch(Counter *c) { c->hits += 1; c->a[1] = 7; return true; }
constexpr int guarded(Counter *c) { contract_assert(touch(c)); return c->hits; }
constexpr int run_guarded() {
  Counter c{0, {1, 2}};
  guarded(&c);
  return guarded(&c) * 100 + c.a[0] * 10 + c.a[1];
}
static_assert(run_guarded() == 12);

// The predicate's own objects can be modified freely.
constexpr int sum(int n) { int s = 0; for (int i = 0; i < n; ++i) s += i; return s; }
constexpr int q(int n) pre(sum(n) < 100) { return n; }
static_assert(q(5) == 5);

// Storage the predicate allocates is freed, even if it does not free it.
constexpr bool heap(int n) { int *p = new int[n]; p[0] = 1; bool r = p[0] == 1; delete[] p; return r; }
constexpr bool leak(int n) { int *p = new int(n); return *p == n; }
constexpr int h(int n) pre(heap(n)) pre(leak(n)) { return n; }
static_assert(h(3) == 3);
