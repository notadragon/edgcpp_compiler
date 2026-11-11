//remark: imported from gcc:p4405-shadow-callee-premises-O0.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=observe --contracts_prove_callee_premises -O0 --contracts_prove_optimize=0
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// DECISIONS.md E199 (b): at proof level 0 there are no proof shadows, so no
// premise copies either: the diagnostics and the object code are the same
// with and without -fcontracts-prove-callee-premises.
//
// Mirror: clang/test/Contracts/p4405-shadow-callee-premises-O0.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O0 -fcontracts-prove-optimize=0" }
// { dg-additional-options "-fcontract-evaluation-semantic=observe -fcontracts-prove-callee-premises" }

#include <contracts>
using namespace std::contracts::labels;

int idx (int x) post (r : r == 0) { return x; }
void h1 (int x)
{
  int i = idx (x);
  contract_assert<diagnose> (i == 0);	// { dg-warning "cannot be proven" }
}

inline int idx2 (int x)
{
  contract_assert (x == 0);
  return x;
}
void h3 (int x)
{
  int i = idx2 (x);
  contract_assert<diagnose> (i == 0);	// { dg-warning "cannot be proven" }
}

void take (int x) pre (x == 0) {}
void h5 (int x)
{
  take (x);
  contract_assert<diagnose> (x == 0);	// { dg-warning "cannot be proven" }
}
