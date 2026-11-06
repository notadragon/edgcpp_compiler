//remark: imported from gcc:p4405-nested-recursive-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove -O0
//match_regex: Test_name.c:32:[0-9]+: error: 
//match_regex: Test_name.c:41:[0-9]+: error: 
//match_regex: Test_name.c:48:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405, DECISIONS.md E169: p4405-nested-recursive.C at -O0, folding
// alone: g's precondition is not proven either, and both are noted, also
// after the guard.
//
// Mirror: clang/test/Contracts/p4405-nested-recursive.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }
// { dg-additional-options "-fcontract-analysis-semantic=prove" }

int k (int x) pre (x > 0);	// { dg-message "precondition of a call in the predicate cannot be proven" }
int k (int) { return 1; }
int g (int x) pre (k (x) == 1);	// { dg-message "precondition of a call in the predicate cannot be proven" }
int g (int) { return 1; }

bool p (int x) pre (x == 0 || p (x - 1));

int
unprovable (int y)
{
  contract_assert (g (y) == 1);	// { dg-error "contract assertion cannot be proven" }
  return y;
}

int
guarded (int y)
{
  if (y <= 0)
    return 0;
  contract_assert (g (y) == 1);	// { dg-error "contract assertion cannot be proven" }
  return y;
}

int
self (int y)
{
  contract_assert (p (y));	// { dg-error "contract assertion cannot be proven" }
  return y;
}
