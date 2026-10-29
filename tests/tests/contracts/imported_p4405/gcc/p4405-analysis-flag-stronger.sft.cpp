//remark: imported from gcc:p4405-analysis-flag-stronger.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=diagnose --contract_analysis_semantic=prove
//match_regex: Test_name.c:22:[0-9]+: error: 
//match_regex: Test_name.c:24:[0-9]+: error: 
//match_regex: Test_name.c:30:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405, step 2 of "Adjusting to the Allowed Semantics": a prove or diagnose
// configured for a runtime evaluation moves to the analysis evaluation,
// which takes the stronger of it and its own configured semantic (ignore <
// diagnose < prove); the runtime evaluation becomes ignore.  Here the
// runtime evaluations are configured diagnose and the analysis evaluation
// prove, so each unproven assertion is an error, not a warning.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=diagnose -fcontract-analysis-semantic=prove" }

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
