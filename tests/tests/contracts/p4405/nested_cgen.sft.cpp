//remark:contracts: P4405 with the C-generating back end: an analysis evaluation depends on the analysis evaluations of the preconditions of the calls its predicate makes, recursively, whether or not the predicate is evaluated at run time (DECISIONS.md E169 in notadragon_wg21); each nested one is decided by folding its precondition with the call's arguments substituted where they fold, a prove one not proven is noted on the outer's diagnostic ("precondition of a call in the predicate cannot be proven" / "... is always false") and never reported on its own, a diagnose one not proven is warned at its call, once per outer evaluation (E169 (9), (11): not again at the same call in the check of the same assertion; see report_once_cgen); the walk skips unevaluated operands, lambdas and the operands of &&, || and ?: that folding shows are not evaluated, takes the calls constant evaluation folded into a constant, and makes no precondition twice on one chain
//require:DO_IL_LOWERING 1
//type:fn
//options_sep:!
//options:--contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove!--contracts_p4405 --contract_analysis_semantic=prove '--contract_configuration=[{"match":{"analysis":true,"namespace":"dd"},"output":{"semantic":"diagnose"}},{"match":{"kind":"pre"},"output":{"semantic":"ignore"}},{"output":{"semantic":"quick_enforce"}}]'!--contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove '--contract_configuration=[{"match":{"analysis":true,"namespace":"dd"},"output":{"semantic":"diagnose"}}]'!--contracts_p4405 --no_contracts_prove_fold --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove!--contracts_p4405 --contract_evaluation_semantic=quick_enforce --contract_analysis_semantic=prove
//match_regex:line ([0-9]+): (error|warning): (contract assertion cannot be proven|contract predicate is always false)
//match_regex:(precondition of a call in the predicate (cannot be proven|is always false) at line [0-9]+)
// (No system headers: the C-generating back end cannot find them natively.)
// The first options analyze everything with prove and check nothing; the
// second check the assertions (quick_enforce) but not the preconditions,
// so each call in a checked predicate also has its own analysis evaluation
// (two evaluations, two diagnostics, E169 (5)), and dd's preconditions are
// analyzed with diagnose; the third analyze dd's with diagnose and check
// nothing; the fourth fold nothing, so every nested precondition is noted
// as unproven, never always false; the fifth check everything, so f's
// precondition is also checked in f, where g1's is analyzed at its call in
// the check, and each call in a checked assertion is analyzed there even
// after the assertion's own evaluation (Clang's checked_outer: no proof
// shadow makes the one a premise for the other).  Nothing proves an outer
// assertion (EDG-134), so each is diagnosed; what differs is its notes.
// Mirrors: GCC's p4405-nested-{prove,diagnose,checked,caller-side,
// recursive}-O0.C (imported in imported_p4405/gcc), Clang's
// p4405-nested-checked.cpp (report_once) and p4405-nested-caller-side.cpp
// (checked_outer).

int g1(int x) pre(x != 0);       // Unprovable for an unknown argument.
int g1(int) { return 1; }
int g3(int x) pre(x != 0);       // Always false for g3(0).
int g3(int x) { return 10 / x; }
int g4(int x) pre(x > 0);        // Proven for g4(5).
int g4(int) { return 1; }
constexpr int c(int x) pre(x != 0) { return 1; }
int k(int x) pre(x > 0);
int k(int) { return 1; }
int g(int x) pre(k(x) == 1);     // A precondition in a precondition's
int g(int) { return 1; }         // predicate: both noted.
bool p(int x) pre(x == 0 || p(x - 1));  // Not made again on the chain.
namespace dd {
int gd(int x) pre(x != 0);
int gd(int) { return 1; }
}
struct S {
  S(int x) pre(x > 0) { }
  int m(int x) const pre(x != 0) { return 1; }
};

int unprovable(int y) { contract_assert(g1(y) == 1); return y; }
int always_false(int y) { contract_assert(g3(0) != 7); return y; }
int provable(int y) { contract_assert(g4(5) == 1); return y; }
// Constant evaluation folds c(0) == 1 under ignore (the precondition is
// not checked there); its precondition is still analyzed.
int folded_call(int y) { contract_assert(c(0) == 1); return y; }
int folded_ok(int y) { contract_assert(c(1) == 1); return y; }
int folded_true(int y) { contract_assert((g1(y), true)); return y; }
int short_circuit(int y) { contract_assert(true || g1(0) == 1); return y; }
int conditional(int y) {
  // g3(0) is not evaluated: no nested evaluation, and in the check (the
  // second and fifth options) no evaluation at its call (code never
  // evaluated, under --contract_analysis_skip_dead_calls, which
  // --contracts_p4405 implies).
  contract_assert((1 ? g4(1) : g3(0)) == 1);
  return y;
}
int unevaluated(int y) {
  contract_assert(sizeof(g1(0)) == sizeof(int));
  return y;
}
int in_lambda(int y) {
  // The lambda's body is its own function, analyzed there.
  contract_assert([] { return g4(2); }() == 1);
  return y;
}
int member(const S &s, int y) { contract_assert(s.m(0) == 1); return y; }
int constructor(int y) { contract_assert((S(0), true)); return y; }
int recursive(int y) { contract_assert(g(y) == 1); return y; }
int self(int y) { contract_assert(p(y)); return y; }
int post_nested(int y) post(r: g1(r) == 1) { return y; }

// A precondition's analysis evaluation at a call, with the call's
// arguments substituted in the nested evaluations.
int f(int y) pre(g1(y) == 1) { return y; }
int caller(int y) { return f(y); }      // g1's noted.
int caller_zero() { return f(0); }      // g1's always false.
int caller_one() { return f(1); }       // g1's proven.

// A nested diagnose precondition (the second and third options): warned at
// its call, once, also when the predicate is checked.
int report_once(int y) { contract_assert(dd::gd(y) == 1); return y; }
int report_zero(int y) { contract_assert(dd::gd(0) == 1); return y; }
