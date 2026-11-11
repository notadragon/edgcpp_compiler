//remark:contracts: P4405 through edg-gxx: a function's own precondition whose predicate calls a function is a premise in its body, so the body's call c.get() is proven (conditionally) and not diagnosed (formerly EDG-188, GCC-1003)
//type:cp
//require:BACK_END_IS_CP_GEN_BE 1
//options:--c++26 --contracts --contracts_p3400 --contracts_p4405 -O2
// D4405R0, "Previous Assertions and Undefined Behavior": the body's call of
// get, whose precondition is what each use's precondition says, is proven
// in use1 (the predicate calls get, which has a precondition of its own),
// use2 (an inline accessor with no contract assertions) and use3 (the
// member read directly).  The proof shadow is g++'s (cpfe-cp puts the
// assertions out as written).  Sound and conditional proofs:
// own_pre_call_premise_sound_gxx.
// Mirror: g++.dg/contracts/cpp26/p4405-own-pre-call-premise.C in the
// gnu_gcc fork.
#include <contracts>
using namespace std::contracts::labels;
struct C {
  int *p;
  int *get() const pre<always_prove>(p) { return p; }
  int *raw() const { return p; }
};
void use1(C& c) pre<always_prove>(c.get() != nullptr) { c.get(); }  // OK
void use2(C& c) pre<always_prove>(c.raw() != nullptr) { c.get(); }  // OK
void use3(C& c) pre<always_prove>(c.p != nullptr) { c.get(); }      // OK
