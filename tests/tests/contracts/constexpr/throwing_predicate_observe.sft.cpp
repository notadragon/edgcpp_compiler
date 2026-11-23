//remark:contracts: a contract predicate that exits via an exception in constant evaluation is a warning under observe, and the value stands
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--contract_evaluation_semantic=observe
//match_regex:line 35: warning: contract condition is not constant
//match_regex:line 39: warning: contract condition is not constant
//match_regex:line 43: warning: contract condition is not constant
//match_regex:line 47: warning: contract condition is not constant
//match_regex:line 52: warning: contract condition is not constant
//match_regex:line 68: warning: contract condition is not constant
// A predicate that exits via an exception is a contract violation
// ([basic.contract.eval]); under observe it is diagnosed at the assertion
// and the evaluation continues, so each static_assert holds.  Both shapes,
// in contract_assert, pre and post, and an exception type owning heap
// storage.  EDG does not implement P3068, so the predicate is diagnosed as
// not constant.  The C-generating back end does not support observe.
// Mirror: g++.dg/contracts/cpp26/contract-constexpr-throwing-predicate-observe.C
// Mirror: clang/test/Contracts/contract-constexpr-throwing-predicate-observe.cpp
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

// Each returns 3 only if the evaluation continues past the violation.
constexpr int inline_throw() {
  contract_assert((throw 1, true));                    // Warning
  return 3;
}
constexpr int throwing_call() {
  contract_assert(thr(1));                             // Warning
  return 3;
}
constexpr int owning_exception() {
  contract_assert((throw owner(), true));              // Warning
  return 3;
}
constexpr int pre_throw(int x)
  pre((throw x, true))                                 // Warning
{
  return 3;
}
constexpr int post_throw(const int x)
  post(thr(x))                                         // Warning
{
  return 3;
}

static_assert(inline_throw() == 3);
static_assert(throwing_call() == 3);
static_assert(owning_exception() == 3);
static_assert(pre_throw(1) == 3);
static_assert(post_throw(1) == 3);

// CONTROL: an exception the predicate catches itself does not exit it, so
// with P3068 (constexpr exceptions) the predicate is true.  EDG does not
// implement P3068: the throw is not a core constant expression, which is
// itself a violation.  When it does, the expected warning below goes away.
constexpr int caught_inside() {
  contract_assert([] { try { throw 1; } catch (int) { return true; } }());
  return 3;
}
static_assert(caught_inside() == 3);
