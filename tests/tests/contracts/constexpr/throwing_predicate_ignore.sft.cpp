//remark:contracts: a contract predicate that would exit via an exception is not evaluated in constant evaluation under ignore
//type:fp
//options_all:--contract_evaluation_semantic=ignore
// Also runs under the C-generating back end.  Nothing is evaluated, so
// nothing is diagnosed and each static_assert holds.
// Mirror: g++.dg/contracts/cpp26/contract-constexpr-throwing-predicate-ignore.C
// Mirror: clang/test/Contracts/contract-constexpr-throwing-predicate-ignore.cpp
constexpr bool thr(int x) {
  if (x != 0)
    throw x;
  return true;
}

// Owns heap storage, freed only by its destructor.
struct owner {
  int *p;
  constexpr owner() : p(new int(1)) {}
  constexpr owner(const owner &o) : p(new int(*o.p)) {}
  constexpr ~owner() { delete p; }
};

constexpr int inline_throw() {
  contract_assert((throw 1, true));                    // OK
  return 3;
}
constexpr int throwing_call() {
  contract_assert(thr(1));                             // OK
  return 3;
}
constexpr int owning_exception() {
  contract_assert((throw owner(), true));              // OK
  return 3;
}
constexpr int pre_throw(int x)
  pre((throw x, true))                                 // OK
{
  return 3;
}
constexpr int post_throw(const int x)
  post(thr(x))                                         // OK
{
  return 3;
}
constexpr int caught_inside() {
  contract_assert([] { try { throw 1; } catch (int) { return true; } }());
  return 3;
}

static_assert(inline_throw() == 3);
static_assert(throwing_call() == 3);
static_assert(owning_exception() == 3);
static_assert(pre_throw(1) == 3);
static_assert(post_throw(1) == 3);
static_assert(caught_inside() == 3);
