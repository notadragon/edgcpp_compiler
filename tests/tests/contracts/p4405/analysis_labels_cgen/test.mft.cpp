//remark:contracts: P4405 with P3400 labels, with the C-generating back end: the allowed set splits into its run-time part R and analysis part N, the analysis evaluation allowing N (and ignore if R is not empty) and the run-time ones R (or ignore if R is empty); the analysis evaluation takes the stronger of its configured semantic and a label's analysis_semantic facet, and a compute_semantic result of prove moves to it; so always_prove's precondition is proven at each call (an error) and taking its function's address is an error (always_diagnose's a warning; nothing for a label that allows a run-time semantic), terminating is analyzed only under prove (and still checked), symbolic as configured; a label allowing only quick_enforce needs no configured quick_enforce to be checked (EDG-142)
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=diagnose:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=diagnose
//match_regex:line ([0-9]+): (error|warning): (contract assertion cannot be proven.*|contract predicate is always false.*|.*__builtin_trap.*)
// The precondition of each function is analyzed at its call in use()
// (the line of that call is the one diagnosed); the analysis semantic
// each label gives, under the configured analysis semantic ignore,
// diagnose and prove (and under diagnose configured for the run-time
// evaluation, which moves it to the analysis evaluation):
//
//                     ignore    diagnose  prove     (diagnose)
//   always_prove      prove     prove     prove     prove
//   always_diagnose   diagnose  diagnose  diagnose  diagnose
//   terminating       ignore    ignore    prove     ignore
//   symbolic          ignore    diagnose  prove     diagnose
//   diagnosed         diagnose  diagnose  prove     diagnose
//   proven            prove     prove     prove     prove
//   computes_prove    prove     prove     prove     prove
//   qe_only           ignore    ignore    ignore    ignore
//
// Taking the address of a: an error; of b: a warning; of c, whose label
// allows run-time semantics: nothing.
// (No system headers: the C-generating back end cannot find them natively.)
#include "analysis_stub.h"
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using enum evaluation_semantic;

template <evaluation_semantic... S>
struct allow {
  using assertion_control_object = allow;
  static constexpr evaluation_semantic_set allowed_semantics{S...};
};
template <evaluation_semantic A>
struct analyzed {
  using assertion_control_object = analyzed;
  constexpr evaluation_semantic analysis_semantic() const { return A; }
};
struct computes_prove_t {
  using assertion_control_object = computes_prove_t;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return prove; }
};
constexpr allow<prove> always_prove{};
constexpr allow<diagnose> always_diagnose{};
constexpr allow<enforce, quick_enforce, prove> terminating{};
constexpr allow<ignore, diagnose, prove> symbolic{};
constexpr analyzed<diagnose> diagnosed{};
constexpr analyzed<prove> proven{};
constexpr computes_prove_t computes_prove{};
constexpr allow<quick_enforce> qe_only{};

int a(int x) pre<always_prove>(x > 0) { return x; }
int b(int x) pre<always_diagnose>(x > 0) { return x; }
int c(int x) pre<terminating>(x > 0) { return x; }
int d(int x) pre<symbolic>(x > 0) { return x; }
int e(int x) pre<diagnosed>(x > 0) { return x; }
int f(int x) pre<proven>(x > 0) { return x; }
int g(int x) pre<computes_prove>(x > 0) { return x; }
int h(int x) pre<qe_only>(x > 0) { return x; }

int use(int y) {
  int r = a(y);
  r += b(y);
  r += c(y);
  r += d(y);
  r += e(y);
  r += f(y);
  r += g(y);
  r += h(y);
  return r;
}
int (*pa)(int) = a;
int (*pb)(int) = b;
int (*pc)(int) = c;
