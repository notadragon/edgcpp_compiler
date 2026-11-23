//remark: imported from gcc:p4405-prove-optimize-no-fold.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contracts_prove_optimize=0 --no_contracts_prove_fold --contract_evaluation_semantic=prove
//match_regex: Test_name.c:18:[0-9]+: error: 
//match_regex: Test_name.c:19:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E205):
// -fno-contracts-prove-fold wins over -fcontracts-prove-optimize=0, where
// folding is the only strategy on, so nothing is proven.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontracts-prove-optimize=0 -fno-contracts-prove-fold -fcontract-evaluation-semantic=prove" }

void f ()
{
  contract_assert (true);	// { dg-error "contract assertion cannot be proven" }
  contract_assert (false);	// { dg-error "contract assertion cannot be proven" }
}
