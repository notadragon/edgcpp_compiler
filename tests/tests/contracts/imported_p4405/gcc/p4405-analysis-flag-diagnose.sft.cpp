//remark: imported from gcc:p4405-analysis-flag-diagnose.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=diagnose
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405: -fcontract-analysis-semantic=diagnose gives the analysis evaluation
// of every contract assertion the diagnose semantic, leaving its runtime
// evaluations to -fcontract-evaluation-semantic (here ignore): a
// precondition (at each call), a postcondition and an assertion each get a
// -Wcontract-unproven warning.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=ignore -fcontract-analysis-semantic=diagnose" }

int f (int x)
  pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
  post (r: r > 0)		// { dg-warning "contract assertion cannot be proven" }
{
  contract_assert (x != 3);	// { dg-warning "contract assertion cannot be proven" }
  return x;
}

int use (int y)
{
  return f (y);			// { dg-warning "contract assertion cannot be proven \\\[-Wcontract-unproven\\\]" }
}
