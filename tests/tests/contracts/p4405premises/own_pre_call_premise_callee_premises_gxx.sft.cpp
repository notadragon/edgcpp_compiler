//remark:contracts: P4405 through edg-gxx: an observe precondition whose predicate calls a function is a premise in g++'s premise copy of its function too, so a caller's evaluation resting on it is proven conditionally and not diagnosed
//type:cp
//require:BACK_END_IS_CP_GEN_BE 1
//options:--c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=observe -fcontracts-prove-assume -O2
// take1's predicate calls an inline accessor, take2's an opaque function.
// h1's and h2's evaluations are left unproven by g++'s ordinary shadow and
// proven, conditionally, by its premise shadow (callee premises, implied
// by --contracts_p4405); a conditional proof is never assumed (g++'s dump
// only).
// Mirror: g++.dg/contracts/cpp26/p4405-own-pre-call-premise-callee-premises.C
// in the gnu_gcc fork.
#include <contracts>
using namespace std::contracts::labels;

bool opaque(int);

struct C {
  int n;
  int get() const { return n; }
};

void take1(const C& c) pre(c.get() > 0) {}
void h1(C& c) { take1(c); contract_assert<prove>(c.n > 0); }  // OK

void take2(int x) pre(opaque(x) && x > 0) {}
void h2(int x) { take2(x); contract_assert<prove>(x > 0); }   // OK
