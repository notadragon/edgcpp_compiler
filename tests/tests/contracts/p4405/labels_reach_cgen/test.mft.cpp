//remark:contracts: P4405 with P3400 labels, with the C-generating back end: the analysis evaluation of each precondition, at its call, has the semantic its label allows from the configured one -- a configured prove or diagnose moves to it, and a best fit reaching ignore's level tries ignore, then diagnose, then prove (from prove: prove, diagnose, ignore; from diagnose: diagnose, ignore, prove), ignore being allowed only when the label allows a run-time semantic -- so a label allowing only prove or only diagnose works under any configured semantic
//require:DO_IL_LOWERING 1
//type:fn
//source_files:labels_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=quick_enforce:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=prove:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=diagnose
//match_regex:line ([0-9]+): (error|warning): contract assertion cannot be proven
// The front end diagnoses each analysis evaluation with prove (an error) or
// diagnose (a warning), a precondition's at each call (in use() below), so
// the diagnostics show the semantic each label gives the analysis
// evaluation (D4405R0, "Adjusting to the Allowed Semantics"; D3595R1,
// "Adjusting a Semantic to the Allowed Set"):
//
//                     ignore    quick_enforce  prove     diagnose
//   proving_only      diagnose  diagnose       prove     diagnose
//   prove_only        prove     prove          prove     prove
//   diagnose_only     diagnose  diagnose       diagnose  diagnose
//   ignore_or_prove   ignore    ignore         prove     ignore
//   qe_or_prove       ignore    ignore         prove     ignore
//   ignore_or_diag    ignore    ignore         diagnose  diagnose
//   always_prove      prove     prove          prove     prove
//
// (qe_or_prove's run-time evaluations are quick_enforce throughout.)
// (No system headers: the C-generating back end cannot find them natively.)
#include "labels_stub.h"
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using enum evaluation_semantic;

template <evaluation_semantic... S>
struct allow {
  using assertion_control_object = allow;
  static constexpr evaluation_semantic_set allowed_semantics{S...};
};
struct always_prove_t {
  using assertion_control_object = always_prove_t;
  static constexpr evaluation_semantic_set allowed_semantics{prove};
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return prove; }
};
constexpr allow<prove, diagnose> proving_only{};
constexpr allow<prove> prove_only{};
constexpr allow<diagnose> diagnose_only{};
constexpr allow<ignore, prove> ignore_or_prove{};
constexpr allow<quick_enforce, prove> qe_or_prove{};
constexpr allow<ignore, diagnose> ignore_or_diag{};
constexpr always_prove_t always_prove{};

int a(int x) pre<proving_only>(x > 0) { return x; }
int b(int x) pre<prove_only>(x > 0) { return x; }
int c(int x) pre<diagnose_only>(x > 0) { return x; }
int d(int x) pre<ignore_or_prove>(x > 0) { return x; }
int e(int x) pre<qe_or_prove>(x > 0) { return x; }
int f(int x) pre<ignore_or_diag>(x > 0) { return x; }
int g(int x) pre<always_prove>(x > 0) { return x; }
int use(int y) {
  int r = a(y);
  r += b(y);
  r += c(y);
  r += d(y);
  r += e(y);
  r += f(y);
  r += g(y);
  return r;
}
