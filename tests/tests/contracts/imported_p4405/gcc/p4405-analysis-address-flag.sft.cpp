//remark: imported from gcc:p4405-analysis-address-flag.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_analysis_semantic=prove
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405 ("Taking the Address of a Function"): with
// -fcontract-analysis-semantic=prove and no label, every precondition
// allows a runtime semantic, which the callee evaluates as its own
// translation unit configures, so taking the address of the function is
// not diagnosed: the configuration of the translation unit taking the
// address does not determine what calls through it are.  A call through
// the pointer is not analyzed.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-analysis-semantic=prove" }

int f (int x) pre (x > 0);			// { dg-bogus "precondition here" }
int g (int x);

int use (int y)
{
  int (*p) (int) = f;				// { dg-bogus "address" }
  int (*q) (int) = g;				// { dg-bogus "address" }
  int (&r) (int) = f;				// { dg-bogus "address" }
  return p (y) + q (y) + r (y);			// { dg-bogus "cannot be proven" }
}
