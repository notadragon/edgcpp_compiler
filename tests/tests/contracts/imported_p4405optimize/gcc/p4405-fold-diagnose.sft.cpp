//remark: imported from gcc:p4405-fold-diagnose.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=diagnose --contracts_prove_optimize=0
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405 stage 2, -fcontracts-prove-fold: as p4405-fold-prove.C with the
// diagnose semantic -- a -Wcontract-unproven warning, for "always false" as
// for "cannot be proven", and nothing for a predicate that folds to true.
//
// Mirror: clang/test/Contracts/p4405-fold-diagnose.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=diagnose -fcontracts-prove-optimize=0" }

int f (int x)
  pre (true)				// { dg-bogus "(cannot be proven|always false)" }
  pre (x > 0)				// { dg-message "the predicate of the contract assertion" }
  post (r: sizeof (r) == 0)		// { dg-warning "contract predicate is always false" }
{
  contract_assert (sizeof (int) >= 1);	// { dg-bogus "(cannot be proven|always false)" }
  contract_assert (false);		// { dg-warning "contract predicate is always false" }
  contract_assert (x != 3);		// { dg-warning "contract assertion cannot be proven" }
  return x;
}

int g (int y)
{
  return f (y)				// { dg-warning "contract assertion cannot be proven" }
	 + f (1);			// { dg-bogus "(cannot be proven|always false)" }
}
