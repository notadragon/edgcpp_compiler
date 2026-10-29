//remark:contracts: a predicate's deallocations do not persist in constant evaluation
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 58: error: contract condition is not constant
//match_regex:line 60: error: contract condition is not constant
//match_regex:line 70: error: expression must have a constant value
// Also runs under the C-generating back end.  A predicate that frees storage
// allocated before it began is constant, and the deallocation is undone
// with the predicate's other side effects once its value is known (EDG-19).
constexpr bool release(int *p) { delete p; return true; }

// The storage is still allocated after the precondition: the caller frees it.
constexpr int r(int *p) pre(release(p)) { return 0; }
constexpr int run_release() {
  int *p = new int(1);
  int v = r(p);
  v += *p;
  delete p;
  return v;
}
static_assert(run_release() == 1);

// The destroyed object, and what its destructor modified, are restored.
struct B {
  int v;
  int *count;
  constexpr ~B() { v = -1; ++*count; }
};
constexpr bool drop(B *b) { delete b; return true; }
constexpr bool drop_array(int *a) { delete[] a; return true; }
constexpr int run_class() {
  int count = 0;
  B *b = new B{5, &count};
  int *a = new int[3]{1, 2, 3};
  contract_assert(drop(b));
  contract_assert(drop_array(a));
  int v = b->v * 100 + count * 10 + a[2];
  delete b;
  delete[] a;
  return v * 10 + count;
}
static_assert(run_class() == 5031);

// A deallocation in a nested predicate, of storage allocated by the
// enclosing one, is undone at the end of the nested predicate; the
// enclosing predicate's storage is freed at its end.
constexpr int inner(int *p) pre(release(p)) { return *p; }
constexpr bool outer() { int *p = new int(4); return inner(p) == 4; }
constexpr int nested() pre(outer()) post(release(new int(2))) { return 7; }
static_assert(nested() == 7);

// The storage counts as deallocated for the rest of the predicate.
constexpr bool use_after(int *p) { delete p; return *p == 1; }
constexpr bool twice(int *p) { delete p; delete p; return true; }
constexpr int run_use_after(bool second) {
  int *p = new int(1);
  if (second) {
    contract_assert(twice(p));
  } else {
    contract_assert(use_after(p));
  }
  delete p;
  return 0;
}
static_assert(run_use_after(false) == 0);
static_assert(run_use_after(true) == 0);

// A deallocation by the predicate alone leaves the storage allocated.
constexpr int leak() { int *p = new int(1); return r(p); }
static_assert(leak() == 0);
