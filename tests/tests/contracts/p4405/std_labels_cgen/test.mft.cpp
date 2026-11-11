//remark:contracts: P4405 with the C-generating back end: the standard labels prove, diagnose (an analysis_semantic facet only) and always_prove, always_diagnose (allowed set {prove}, {diagnose}), as <contracts> declares them, decide the analysis evaluation of a precondition (at each call), a postcondition and a contract_assert, named unqualified (the contract_control namespace) or qualified; prove | diagnose is prove; the front end proves nothing, not even a tautology (EDG-134: g++ folds them through edg-gxx); diagnose combined with a label allowing only run-time semantics (quick_enforce, the C-generating back end's stand-in for always_enforce, and terminating) is adjusted to the combined allowed set, where diagnose is not, so its analysis draws nothing (DECISIONS.md E198); the configured run-time semantic (quick_enforce, then ignore) changes nothing; without --contracts_p4405 the labels are not declared (each name an error); as GCC's p4405-std-labels.C and p4405-std-labels-without-flag.C
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=quick_enforce:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore:--contracts_p3400 --contract_evaluation_semantic=ignore
//match_regex:line ([0-9]+): (error|warning): (contract .*|no valid evaluation semantic.*|identifier .* is undefined|.* has no member .*)
// (No system headers: the C-generating back end cannot find them natively;
// analysis_stub.h stands in for <contracts>.)  The front end proves nothing
// of a call's argument (EDG-134), so each unguarded use is diagnosed: an
// error with prove, a warning with diagnose.
#include "analysis_stub.h"
namespace L = std::contracts::labels;
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

// The run-time semantics the E198 combinations restrict to: terminating's,
// as <contracts> declares it, and only quick_enforce (always_enforce allows
// only enforce, which this back end does not check).
struct always_qe_t {
  using assertion_control_object = always_qe_t;
  static constexpr evaluation_semantic_set allowed_semantics{
    evaluation_semantic::quick_enforce};
};
constexpr always_qe_t always_qe{};
struct terminating_t {
  using assertion_control_object = terminating_t;
  static constexpr evaluation_semantic_set allowed_semantics{
    evaluation_semantic::enforce, evaluation_semantic::quick_enforce,
    evaluation_semantic::prove};
};
constexpr terminating_t terminating_{};

void pp(int x) pre<prove>(x > 0);
void pd(int x) pre<diagnose>(x > 0);
void pap(int x) pre<always_prove>(x > 0);
void pad(int x) pre<always_diagnose>(x > 0);
void ppd(int x) pre<prove | diagnose>(x > 0);
void pdq(int x) pre<L::diagnose>(x > 0);

void calls(int y) {
  pp(y);
  pd(y);
  pap(y);
  pad(y);
  ppd(y);
  pdq(y);
}

int qp(int x) post<prove>(r: r > 0) { return x; }
int qd(int x) post<diagnose>(r: r > 0) { return x; }
int qap(int x) post<always_prove>(r: r > 0) { return x; }
int qad(int x) post<always_diagnose>(r: r > 0) { return x; }

void asserts(int x) {
  contract_assert<prove>(x > 0);
  contract_assert<diagnose>(x > 0);
  contract_assert<always_prove>(x > 0);
  contract_assert<always_diagnose>(x > 0);
}

// Provable, but the front end proves nothing (EDG-134; GCC: nothing).
void proven(int x) {
  contract_assert<prove>(sizeof(x) == sizeof(int));
  contract_assert<always_prove>(true);
  contract_assert<diagnose>(1 + 1 == 2);
}

// E198: the analysis draws nothing.
void ae(int x) pre<diagnose | always_qe>(x > 0);
void at(int x) pre<diagnose | terminating_>(x > 0);

void restricted(int y) {
  ae(y);
  at(y);
  contract_assert<diagnose | always_qe>(y > 0);
  contract_assert<diagnose | terminating_>(y > 0);
}
