//remark: imported from gcc:p4405-analysis-flag-stronger-2.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove --contract_analysis_semantic=diagnose
//match_regex: Test_name.c:18:[0-9]+: error: 
//match_regex: Test_name.c:20:[0-9]+: error: 
//match_regex: Test_name.c:26:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405, step 2: a configured runtime prove beats a configured analysis
// diagnose -- the analysis evaluation takes the stronger, prove.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove -fcontract-analysis-semantic=diagnose" }

int f (int x)
  pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
  post (r: r > 0)		// { dg-error "contract assertion cannot be proven" }
{
  contract_assert (x != 3);	// { dg-error "contract assertion cannot be proven" }
  return x;
}

int use (int y)
{
  return f (y);			// { dg-error "contract assertion cannot be proven" }
}
