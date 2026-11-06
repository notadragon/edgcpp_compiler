//remark:contracts: EDG-81 (stock): a lambda captures this from an enclosing lambda's copy of *this, also where this has no variable (a default member initializer)
//type:rp
//options_all:--contract_evaluation_semantic=quick_enforce
// Not contracts-specific; the predicate of a declaration has no "this"
// variable either.  No system headers (also runs under the C-generating
// back end).
struct N {
  int m = 1;
  int g = [*this]() mutable { return [this] { return ++m; }(); }();
  int h = [*this] { return [this] { return m + 1; }(); }();
  N *p = [&] { return [=] { return this; }(); }();
  int q = [=] { return [*this] { return m + 1; }(); }();
};
struct P {
  int m = 1;
  constexpr int f() const pre([=] { return [*this] { return m == 1; }(); }()) {
    return m;
  }
};
static_assert(P{}.f() == 1);
struct S {
  int m;
  int f() { return [*this]() mutable { return [this] { return ++m; }(); }(); }
};
int main() {
  N n;
  S s{1};
  return (n.g == 2 && n.h == 2 && n.p == &n && n.q == 2 &&
          s.f() == 2 && s.m == 1) ? 0 : 1;
}
