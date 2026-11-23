//remark: imported from gcc:p4405-analysis-flag-prove.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove
//match_regex: Test_name.c:20:[0-9]+: error: 
//match_regex: Test_name.c:22:[0-9]+: error: 
//match_regex: Test_name.c:29:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405: -fcontract-analysis-semantic=prove gives the analysis evaluation of
// every contract assertion the prove semantic: each unproven one is an
// error, a precondition at each call.  The runtime evaluations are ignore (-fcontract-evaluation-semantic).
// A predicate that folds to true is proven.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=ignore -fcontract-analysis-semantic=prove" }

int f (int x)
  pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
  post (r: r > 0)		// { dg-error "contract assertion cannot be proven" }
{
  contract_assert (x != 3);	// { dg-error "contract assertion cannot be proven" }
  contract_assert (sizeof (int) > 0);	// { dg-bogus "cannot be proven" }
  return x;
}

int g (int y)
{
  return f (y)			// { dg-error "contract assertion cannot be proven" }
	 + f (1);		// { dg-bogus "cannot be proven" }
}
