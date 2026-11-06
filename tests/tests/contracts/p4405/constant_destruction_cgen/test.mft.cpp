//remark:contracts: P4405 with the C-generating back end: constant evaluation has no analysis evaluation (D4405R0, "The Analysis Evaluation"), so neither the destructor call of a constexpr variable's constant destruction nor the destructor calls of the temporaries made while initializing a constexpr or constinit variable are analyzed, at namespace scope, as a member, a static member, a variable template, a function's automatic and static variables and a template's; an object destroyed at run time still is -- an automatic one where it is destroyed, and one of static storage duration through its destructor's address (E146), once each.  Mirror: gnu_gcc g++.dg/contracts/cpp26/p4405-analysis-constant-destruction-cgen.C
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=ignore
//match_regex:line ([0-9]+): (error|warning): (contract .*)
// (No system headers: the C-generating back end cannot find them natively.)
// The precondition holds in constant evaluation and folding outside it does
// not prove it, so any analysis of these calls would be an error.
// Differences from GCC: `constinit DE ci' is not diagnosed -- the front end
// does not register its destruction, whose evaluation is constant, so no
// call is made through the destructor's address (DECISIONS.md E149 (a): each
// implementation diagnoses the objects it destroys; Clang does the same);
// H's implicit destructor's call of ~DE is not analyzed (calls in implicitly
// defined special member functions, EDG-134).
#include "analysis_stub.h"
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using enum evaluation_semantic;

struct always_prove_t {
  using assertion_control_object = always_prove_t;
  static constexpr evaluation_semantic_set allowed_semantics{prove};
};
constexpr always_prove_t always_prove{};

int g;

struct DE {
  constexpr ~DE() pre<always_prove>(__builtin_is_constant_evaluated() ||
                                    g > 0) {}
};
struct H { DE d; };
struct HA { DE d; int i; };
struct V { int v; constexpr V(const DE &) : v(1) {} };

constexpr DE ce1;                // Constant destruction: nothing.
constexpr DE ce2{};
constexpr DE ce3 = DE();
constexpr DE ca[2];
constexpr H ch;
constexpr HA cha{DE(), 1};
constexpr const DE &cr = ce1;
constexpr int ci1 = (DE(), 1);   // Temporaries of a constant
constinit int ci2 = (DE(), 2);   // initialization: nothing.
constexpr V cv{DE()};
constinit DE ci;                 // Not destroyed: nothing.
DE e1;                           // An error, through the address.

struct S { static constexpr DE sm{}; };
template <class T> constexpr T vt{};
template <class T> inline void tf() {
  constexpr T a;
  static constexpr T s;
  constexpr int t = (T(), 1);
}

void f() {
  constexpr DE a;
  constexpr DE b{};
  constexpr DE c[2];
  static constexpr DE s;
  constexpr int l = (DE(), 3);
  static constinit int m = (DE(), 4);
  (void)a; (void)b; (void)c; (void)s; (void)l; (void)m;
  (void)&vt<DE>;
  (void)&S::sm;
  tf<DE>();
}

void h() {
  DE d;                          // An error, where d is destroyed.
}

void hh() {
  H h2;                          // EDG-134: nothing.
}
