//remark:contracts: a contract predicate that exits via an exception in constant evaluation is an error under enforce
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--contract_evaluation_semantic=enforce
//match_regex:line 37: error: contract condition is not constant
//match_regex:line 41: error: contract condition is not constant
//match_regex:line 45: error: contract condition is not constant
//match_regex:line 49: error: contract condition is not constant
//match_regex:line 54: error: contract condition is not constant
//match_regex:line 75: error: contract condition is not constant
// A predicate that exits via an exception is a contract violation
// ([basic.contract.eval]); with a terminating semantic the manifestly
// constant-evaluated expression is ill-formed, and the diagnostic is at the
// assertion.  Both shapes: a throw written in the predicate, and a call that
// throws; in contract_assert, pre and post, and an exception type owning heap
// storage.  EDG does not implement P3068, so a throw is never a core
// constant expression and the predicate is diagnosed as not constant; no
// exception object is created, so none is reported as never freed.
// The C-generating back end does not support enforce.
// Mirror: g++.dg/contracts/cpp26/contract-constexpr-throwing-predicate.C
// Mirror: clang/test/Contracts/contract-constexpr-throwing-predicate.cpp
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

constexpr bool inline_throw() {
  contract_assert((throw 1, true));                    // Error
  return true;
}
constexpr bool throwing_call() {
  contract_assert(thr(1));                             // Error
  return true;
}
constexpr bool owning_exception() {
  contract_assert((throw owner(), true));              // Error
  return true;
}
constexpr bool pre_throw(int x)
  pre((throw x, true))                                 // Error
{
  return true;
}
constexpr bool post_throw(const int x)
  post(thr(x))                                         // Error
{
  return true;
}
constexpr bool not_thrown() {
  contract_assert(thr(0));                             // OK
  return true;
}

static_assert(inline_throw());
static_assert(throwing_call());
static_assert(owning_exception());
static_assert(pre_throw(1));
static_assert(post_throw(1));
static_assert(not_thrown());

// CONTROL: an exception the predicate catches itself does not exit it, so
// with P3068 (constexpr exceptions) the predicate is true.  EDG does not
// implement P3068: the throw is not a core constant expression, which is
// itself a violation.  When it does, the expected error below goes away.
constexpr bool caught_inside() {
  contract_assert([] { try { throw 1; } catch (int) { return true; } }());
  return true;
}
#if defined(__cpp_constexpr_exceptions)
static_assert(caught_inside());                        // OK
#else
static_assert(caught_inside());                        // Error, at the assertion
#endif

// Not manifestly constant-evaluated: no diagnostic, the violation is left to
// run time.
bool at_run_time() { return inline_throw() && throwing_call(); }
