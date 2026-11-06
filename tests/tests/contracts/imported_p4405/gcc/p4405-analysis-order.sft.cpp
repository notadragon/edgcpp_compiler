//remark: imported from gcc:p4405-analysis-order.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3850 --contract_evaluation_semantic=enforce --contract_analysis_semantic=diagnose '--contract_configuration=[{"match":{"caller":true},"output":{"semantic":"enforce"}}]' -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.  The front
// end has no -fcontracts-client-check=all; P3595's entry for it, a
// "caller" match with the semantic enforce, stands in for it.
// D4405 ("The Analysis Evaluation"): the analysis evaluation is the first
// evaluation of the assertion, so its proof never relies on the
// assertion's own runtime check.  Each assertion below is checked with
// enforce, whose violation does not return, and analyzed with diagnose: had
// the analysis come after the check, the check would prove it.  A later
// assertion of the same predicate is proven by the earlier ones.  At a
// call, the analysis comes before the caller-side check
// (-fcontracts-client-check=all) as well.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3850 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=enforce -fcontract-analysis-semantic=diagnose -fcontracts-client-check=all" }

void a (int x)
{
  contract_assert (x > 0);	// { dg-warning "contract assertion cannot be proven" }
  contract_assert (x > 0);	// { dg-bogus "cannot be proven" }
}

int b (int x)
  post (r: r > 0)		// { dg-warning "contract assertion cannot be proven" }
{
  return x;
}

int c (int x) pre (x > 0);	// { dg-message "the predicate of the contract assertion" }

int d (int y)
{
  int r = c (y);		// { dg-warning "contract assertion cannot be proven" }
  return r + c (y);		// { dg-bogus "cannot be proven" }
}
