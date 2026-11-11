//remark: imported from gcc:p4405-shadow-callee-premises-ignore.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore -O2
//match_regex: Test_name.c:29:[0-9]+: error: 
//match_regex: Test_name.c:53:[0-9]+: error: 
//match_regex: Test_name.c:60:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// p4405-shadow-callee-premises.C with ignore: the contract_assert in the
// inline callee (h3) is a premise through its copy, as one in the function's
// own body is (h2).  An ignore postcondition or precondition leaves no region
// in its function at all (GCC-885), so h1, h4 and h5 are still errors.
//
// Mirror: clang/test/Contracts/p4405-shadow-callee-premises.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }

#include <contracts>
using namespace std::contracts::labels;

int idx (int x) post (r : r == 0) { return x; }
void h1 (int x)
{
  int i = idx (x);
  contract_assert<prove> (i == 0);	// { dg-error "cannot be proven" }
}

void h2 (int x)
{
  contract_assert (x == 0);
  contract_assert<prove> (x == 0);	// { dg-bogus "cannot be proven" }
}

inline int idx2 (int x)
{
  contract_assert (x == 0);
  return x;
}
void h3 (int x)
{
  int i = idx2 (x);
  contract_assert<prove> (i == 0);	// { dg-bogus "cannot be proven" }
}

int k (int x) { return idx (x) + 1; }
void h4 (int x)
{
  int i = k (x);
  contract_assert<prove> (i == 1);	// { dg-error "cannot be proven" }
}

void take (int x) pre (x == 0) {}
void h5 (int x)
{
  take (x);
  contract_assert<prove> (x == 0);	// { dg-error "cannot be proven" }
}

