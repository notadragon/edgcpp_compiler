//remark: imported from gcc:p4405-analysis-flag-ignore.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contract_analysis_semantic=ignore
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405: -fcontract-analysis-semantic=ignore is the default: no analysis.
// Given after =prove, the last one wins.  A prove selected for the runtime
// evaluations by -fcontract-evaluation-semantic still moves to the analysis
// evaluation (the stronger of the two), as p4405-prove.C shows; here the
// runtime evaluations are ignore, so nothing is diagnosed.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=ignore -fcontract-analysis-semantic=prove -fcontract-analysis-semantic=ignore" }

int f (int x)
  pre (x > 0)			// { dg-bogus "cannot be proven" }
  post (r: r > 0)		// { dg-bogus "cannot be proven" }
{
  contract_assert (x != 3);	// { dg-bogus "cannot be proven" }
  return x;
}
