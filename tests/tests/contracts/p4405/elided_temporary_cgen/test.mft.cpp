//remark:contracts: P4405 with the C-generating back end: the destructor call of a temporary that an initialization elides is never made, so it has no analysis evaluation; a temporary that is materialized and destroyed has one -- at namespace scope (the variables of static storage duration still get their address-of error) and in functions: returns, new, conditionals, the comma operator, by-value arguments, an unused inline function, a template's instance; and the cleanup of a member of a partially constructed aggregate, a call made in the initialization, is analyzed whatever the aggregate's storage duration (EDG-150: not at namespace scope, or for a static data member), when it is potentially invoked: some later subobject is initialized, by an initializer or a default member initializer, whether or not that can throw; a later member left to implicit value-initialization does not count, and the last subobject has nothing after it (DECISIONS.md E160 (a); the last subobject's cleanup was analyzed too).  Mirror: gnu_gcc g++.dg/contracts/cpp26/p4405-analysis-elided-temporary.C, p4405-analysis-elided-temporary-aggregate.C
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=ignore
//match_regex:line ([0-9]+): (error|warning): (contract .*)
// (No system headers: the C-generating back end cannot find them natively.)
// Differences from GCC: a delete expression's destructor call is not
// analyzed, nor are the calls of A's implicit destructor (EDG-134).
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

struct DP {
  DP();
  ~DP() pre<always_prove>(g > 0);
};
DP make();
void take(DP);
int thr();
struct A { DP p; int i; };

// Namespace scope.
DP x = DP();                     // Only the address-of errors.
DP y = make();
DP w = g ? DP() : make();
DP *np = new DP(DP());           // Nothing.
DP *mp = new DP(make());
int z = (DP(), 1);               // An error.
int t = (take(DP()), 1);         // An error.
A na{DP(), thr()};               // EDG-150: an error, na.p's cleanup.
A nb{make(), thr()};             // Likewise.
struct SA { static A sa; };
A SA::sa{DP(), thr()};           // Likewise.

// In a function.
DP ret() {
  return DP();                   // Nothing.
}
DP ret_make() {
  return make();                 // Nothing.
}
DP ret_cond() {
  return g ? DP() : make();      // Nothing.
}
void f() {
  DP *p = new DP(DP());          // Nothing.
  DP *q = new DP(make());        // Nothing.
  DP *r = new DP{DP()};          // Nothing.
  int a = (DP(), 1);             // An error.
  take(DP());                    // An error, the parameter's destruction.
  take(make());                  // Likewise.
  DP();                          // An error.
  (void)make();                  // An error.
  A b{DP(), thr()};              // An error, b.p's cleanup.
  static A c{DP(), thr()};       // Likewise.
  (void)p; (void)q; (void)r; (void)a; (void)b; (void)c;
}

// An inline function nobody calls: analyzed like any other.
inline DP unused_ret() {
  return DP();                   // Nothing.
}
inline int unused_kept() {
  return (DP(), 1);              // An error.
}

// A template, in its instance.
template <typename T> inline T tret() {
  return T();                    // Nothing.
}
template <typename T> inline int tkept() {
  return (T(), 1);               // An error.
}
void h() {
  delete new DP(tret<DP>());     // EDG-134: nothing.
  tkept<DP>();
}

// DECISIONS.md E160 (a): the cleanup of an already-built subobject is
// potentially invoked until the aggregate's initialization completes, even
// when nothing after it can throw: a base, a member initialized by
// parentheses, a default member initializer, a capture, an array's element.
int nothr() noexcept;
struct DN {
  DN() noexcept;
  ~DN() pre<always_prove>(g > 0);
};
struct AI { DP p; int i = 5; };
struct L { int i; DP p; };
struct D : DP { int i; ~D(); };
struct AR { DN a[2]; int i; };
struct O { A a; int j; };

A e1{DP(), 1};                   // An error.
A e2{DP(), nothr()};             // An error.
A e3(DP(), 1);                   // An error.
D e4{{}, 1};                     // An error, the base's cleanup.
AI e5{DP()};                     // An error.
A e6{DP()};                      // Nothing: i is value-initialized.
L e7{1, DP()};                   // Nothing: p is the last member.
struct SE { static A se; };
A SE::se{DP(), 1};               // An error.
O e9{{DP()}, 1};                 // An error: j follows a.p.
O e10{{DP()}};                   // Nothing.

void k() {
  A b1{DP(), 1};                 // An error.
  static A b2{DP(), nothr()};    // An error.
  A b3(DP(), 1);                 // An error.
  D b4{{}, 1};                   // An error.
  static AI b5{DP()};            // An error.
  A b6{DP()};                    // Nothing.
  L b7{1, DP()};                 // Nothing.
  auto b8 = [p = DP(), i = 1] {};  // An error, the capture's cleanup.
  auto b11 = [i = 1, p = DP()] {}; // Nothing.
  AR b10{{DN(), DN()}};          // An error, a[0]'s cleanup only.
  (void)b1; (void)b2; (void)b3; (void)b4; (void)b5; (void)b6; (void)b7;
  (void)b8; (void)b11; (void)b10;
}
