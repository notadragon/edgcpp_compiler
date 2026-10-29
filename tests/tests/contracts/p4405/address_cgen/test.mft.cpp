//remark:contracts: P4405 with the C-generating back end: taking the address of a function is diagnosed only for a precondition whose label allows no run-time semantic (always_prove, always_diagnose) and that folding does not prove true: "contract assertion cannot be proven for a call through the address of" it, an error under prove and a warning under diagnose ("contract predicate is always false ..." when it folds to false), with a note for the precondition, and for an object of static or thread storage duration or an extended temporary, destroyed through its destructor's address, a note naming it; a tautological precondition is silent by every form of taking the address, the first unproven precondition is reported (with prove before diagnose), and a precondition analyzed with prove only by the configuration draws nothing (D4405R0's section 4.2 example); converting a closure to a pointer to function takes the address of its call operator (EDG-148), and an unevaluated operand (decltype, sizeof) takes no address and makes no call (EDG-147); with --no_contracts_prove_fold nothing is folded, so every precondition that allows no run-time semantic is diagnosed, never "always false" (DECISIONS.md E159 (a))
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=diagnose:--contracts_p3400 --contracts_p4405 --no_contracts_prove_fold --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove
//match_regex:line ([0-9]+): (error|warning): (contract .*)
// (No system headers: the C-generating back end cannot find them natively.)
// The labels decide; without folding (the third options) tautologies too.
// Folding settles only a predicate the front end folds to a constant (GCC's
// folder also settles x == x); a call's analysis folds nothing (EDG-134).
#include "analysis_stub.h"
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using enum evaluation_semantic;

struct always_prove_t {
  using assertion_control_object = always_prove_t;
  static constexpr evaluation_semantic_set allowed_semantics{prove};
};
constexpr always_prove_t always_prove{};
struct always_diagnose_t {
  using assertion_control_object = always_diagnose_t;
  static constexpr evaluation_semantic_set allowed_semantics{diagnose};
};
constexpr always_diagnose_t always_diagnose{};

constexpr int k = 3;
int n;

// Tautological: proven by folding, so nothing below.
void t1(int x) pre<always_prove>(true);
void t2(int x) pre<always_prove>(sizeof(x) == sizeof(int));
void t3(int x) pre<always_prove>(k > 0);
void t4(int x) pre<always_diagnose>(sizeof(x) == sizeof(int));
void t5(int x) pre<always_prove>(true) pre<always_diagnose>(k == 3);
// Folded to false: "always false".
void z1(int x) pre<always_prove>(false);
void z2(int x) pre<always_diagnose>(k < 0);
// The first unproven one, with prove before diagnose.
void m1(int x) pre<always_prove>(true) pre<always_diagnose>(x > 0)
                pre<always_prove>(x < 9);
void m2(int x) pre<always_prove>(true) pre<always_diagnose>(x > 0);
// D4405R0's example (section 4.2).
void f1(int x) pre<always_prove>(sizeof(x) == sizeof(int));
void f2(int x) pre<always_prove>(x > 0);
void f3(int x) pre<always_diagnose>(x > 0);
void f4(int x) pre(x > 0);  // Analyzed with prove by the configuration.

struct S {
  void mt(int x) pre<always_prove>(sizeof(x) == 4 || sizeof(x) != 4);
  static void st(int x) pre<always_prove>(true);
  void mu(int x) pre<always_prove>(x > 0);
};
template <class T> void tt(T x) pre<always_prove>(sizeof(T) == sizeof(x));
template <class T> void tu(T x) pre<always_prove>(x > 0);

struct DT { ~DT() pre<always_prove>(true); };
struct DK { ~DK() pre<always_diagnose>(k == 3); };
struct DZ { ~DZ() pre<always_prove>(k == 4); };
struct DN { ~DN() pre<always_prove>(n > 0); };
struct DW { ~DW() pre<always_diagnose>(n > 0); };
struct DC { ~DC() pre(n > 0); };  // Configured only.

DT dt1;                          // Tautological: nothing.
DK dk1;
thread_local DT dt2;
DT dta[2];                       // Nothing for the address; its element
                                 // destructor calls are (EDG-134).
const DT &rt = DT();
DZ dz1;                          // Always false.
DN dn1;                          // An error, naming dn1.
thread_local DN dn2;             // Likewise, of thread storage duration.
const DN &rn = DN();             // Likewise, a temporary.
DW dw1;                          // A warning.
DC dc1;                          // Configured only: nothing.

void use() {
  void (*p1)(int) = t1;
  void (*p2)(int) = &t2;
  void (&r3)(int) = t3;
  void (*p4)(int) = t4;
  void (*p5)(int) = t5;
  void (S::*pm)(int) = &S::mt;
  void (*ps)(int) = S::st;
  void (*pt)(int) = tt<int>;
  static DT dt3;
  void (*q1)(int) = z1;          // Always false, an error.
  void (*q2)(int) = z2;          // Always false, a warning.
  void (*q3)(int) = m1;          // m1's third precondition, an error.
  void (*q4)(int) = m2;          // m2's second, a warning.
  void (*g1)(int) = &f1;         // OK, proven tautological.
  void (*g2)(int) = &f2;         // Error, cannot be proven through g2.
  void (*g3)(int) = &f3;         // OK, warns: cannot be proven through g3.
  void (*g4)(int) = &f4;         // OK, calls through g4 are not analyzed.
  void (S::*qm)(int) = &S::mu;   // An error.
  void (*qt)(int) = tu<int>;     // An error.
  (void)p1; (void)p2; (void)r3; (void)p4; (void)p5; (void)pm; (void)ps;
  (void)pt; (void)q1; (void)q2; (void)q3; (void)q4; (void)g1; (void)g2;
  (void)g3; (void)g4; (void)qm; (void)qt;
}

// Converting a closure to a pointer to function takes the address of its
// call operator (EDG-148): a lambda, a static one, a generic one (the
// specialization converted to), by +, a cast, in a template instance.
auto unused = [](int x) pre<always_prove>(x > 0) { return x; };  // Nothing.
int lambdas(int y) {
  auto l1 = [](int x) pre<always_prove>(x > 0) { return x; };
  int (*p1)(int) = l1;           // An error.
  auto l2 = [](int x) static pre<always_prove>(x > 0) { return x; };
  int (*p2)(int) = l2;           // An error.
  auto l3 = [](auto x) pre<always_prove>(x > 0) { return x; };
  int (*p3)(int) = l3;           // An error.
  auto l4 = [](int x) pre<always_diagnose>(x > 0) { return x; };
  int (*p4)(int) = +l4;          // A warning.
  auto p5 = static_cast<int (*)(int)>(l4);   // A warning.
  auto l6 = [](auto x) static pre<always_diagnose>(x > 0) { return x; };
  long (*p6)(long) = l6;         // A warning.
  auto l7 = [](int x) pre(x > 0) { return x; };
  int (*p7)(int) = l7;           // Configured only: nothing.
  auto l8 = [](int x) pre<always_prove>(sizeof(x) == sizeof(int)) {
    return x;
  };
  int (*p8)(int) = l8;           // Tautological: nothing.
  return p1(y) + p2(y) + p3(y) + p4(y) + p5(y) + (int)p6(y) + p7(y) + p8(y);
}
template <class T> T in_template(T y) {
  auto l = [](T x) pre<always_prove>(x > 0) { return x; };
  T (*p)(T) = l;                 // An error, in in_template<int>.
  return p(y);
}
template int in_template<int>(int);

// An unevaluated operand takes no address and makes no call (EDG-147).
int a(int x) pre<always_prove>(x > 0);
int unevaluated(int y) {
  auto l = [](int x) pre<always_prove>(x > 0) { return x; };
  decltype(&f2) d1 = nullptr;
  decltype(+l) d2 = nullptr;
  using T = decltype(+l);
  T d3 = nullptr;
  decltype(a(y)) d4 = sizeof(a(y)) + sizeof(&f2);
  return (d1 ? 1 : 0) + (d2 ? 1 : 0) + (d3 ? 1 : 0) + d4;
}
