//remark:contracts: P4405 through edg-gxx: an observe evaluation in a callee g++'s proof shadow inlines (a postcondition, an inline callee's contract_assert) is a premise, so a prove evaluation resting on it is proven conditionally and not diagnosed, as one resting on an observe evaluation in its own body is (formerly EDG-185, GCC-993)
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=observe -O2
// D4405R0 Section 4.5 (notadragon_wg21 DECISIONS.md E196 item 2, E199 (b)):
// a conditional proof is not diagnosed, so none of h1, h2, h3 is.  The
// proof shadow is g++'s (cpfe-cp puts the assertions out as written); the
// premise copies of h1's and h3's callees are its callee-premises strategy
// (--contracts_prove_callee_premises, implied by --contracts_p4405).
// Without it, and an ignore postcondition (GCC-885, our GCC's): the
// imports in imported_p4405premises/gcc.  Reduced from
// D4405R0-implexample-bindels-expected.
// Mirror: g++.dg/contracts/cpp26/p4405-shadow-callee-premises.C in the
// gnu_gcc fork.
#include <contracts>
using namespace std::contracts::labels;

int idx(int x) post(r : r == 0) { return x; }
void h1(int x) { int i = idx(x); contract_assert<prove>(i == 0); }

void h2(int x) { contract_assert(x == 0); contract_assert<prove>(x == 0); }

inline int idx2(int x) { contract_assert(x == 0); return x; }
void h3(int x) { int i = idx2(x); contract_assert<prove>(i == 0); }
