//remark:contracts: P4405 through edg-gxx: a precondition whose predicate calls a function proves its body's evaluations, soundly or conditionally, but one whose predicate has a side effect of its own is no premise
//type:cn
//require:BACK_END_IS_CP_GEN_BE 1
//options:--c++26 --contracts --contracts_p3400 --contracts_p4405 --contracts_allow_assume --contract_evaluation_semantic=assume -fcontracts-prove-assume -O2
//match_regex:Test_name.c:([0-9]+):[0-9]+: error: (contract assertion cannot be proven)
// D4405R0, "Previous Assertions and Undefined Behavior": every
// precondition here is assume, every body evaluation prove, and each body
// evaluation can be proven only from its function's precondition.  The
// proofs in sound_accessor (an inline accessor, pure in fact) and the
// conditional ones (a callee with a precondition of its own, an opaque
// callee, one with a side effect, one that may throw) are not diagnosed;
// own_effect's predicate assigns through a mutable member, is not assumed,
// and its body evaluation is diagnosed (line 49).  Which proofs are sound
// is in g++'s dump only.
// Mirror: g++.dg/contracts/cpp26/p4405-own-pre-call-premise-sound.C in the
// gnu_gcc fork.
#include <contracts>
using namespace std::contracts::labels;

int sink(int);
bool opaque(const int *);
int counter;

struct C {
  int *p;
  int *raw() const { return p; }
  int *get() const pre(p) { return p; }
  int *bump() const { ++counter; return p; }
  int *thr() const { if (!p) throw 1; return p; }
};

int sound_accessor(C& c) pre(c.raw() != nullptr)
  { contract_assert<prove>(c.p != nullptr); return sink(*c.p); }    // OK

int cond_checked(C& c) pre(c.get() != nullptr)
  { contract_assert<prove>(c.p != nullptr); return sink(*c.p); }    // OK

int cond_opaque(C& c) pre(opaque(c.p) && c.p != nullptr)
  { contract_assert<prove>(c.p != nullptr); return sink(*c.p); }    // OK

int cond_effects(C& c) pre(c.bump() != nullptr)
  { contract_assert<prove>(c.p != nullptr); return sink(*c.p); }    // OK

int cond_throws(C& c) pre(c.thr() != nullptr)
  { contract_assert<prove>(c.p != nullptr); return sink(*c.p); }    // OK

struct M { mutable int k; };
int own_effect(const M& m) pre((m.k += 0) > 0)
  { contract_assert<prove>(m.k > 0); return sink(m.k); }            // Error
