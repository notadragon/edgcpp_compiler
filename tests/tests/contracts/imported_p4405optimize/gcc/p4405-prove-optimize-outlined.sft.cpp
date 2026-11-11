//remark: imported from gcc:p4405-prove-optimize-outlined.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -fcontract-checks-outlined -fcontract-disable-optimized-checks --contract_evaluation_semantic=observe -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E206):
// -fcontract-checks-outlined -fcontract-disable-optimized-checks gives the
// outlined checking functions an optimize ("-O0") attribute, which does
// not alter the proofs (E168 (7)).  idx's observe postcondition is checked
// in its outlined checker, which the premise copies of idx call: the
// checker gets a premise copy too, optimized at the proof level, so the
// premise reaches h1's premise shadow and its evaluation is proven,
// conditionally.  sq's prove precondition is in its checker, whose shadow
// is optimized at the proof level.
//
// Mirror: none (GCC's -fcontract-checks-outlined).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-checks-outlined -fcontract-disable-optimized-checks" }
// { dg-additional-options "-fcontract-evaluation-semantic=observe  " }

#include <contracts>
using namespace std::contracts::labels;

int idx (int x) post (r : r == 0) { return x; }

void h1 (int x)
{
  int i = idx (x);
  contract_assert<prove> (i == 0);	// { dg-bogus "cannot be proven" }
}

int sq (int x) pre<prove> ((x & 1) < 2) { return x; }	// { dg-bogus "cannot be proven" }
int use_sq (int x) { return sq (x); }

//
//
