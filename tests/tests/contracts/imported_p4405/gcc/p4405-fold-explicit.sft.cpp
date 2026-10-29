//remark: imported from gcc:p4405-fold-explicit.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --no_contracts_prove_fold --contracts_prove_fold --contract_evaluation_semantic=prove
//match_regex: Test_name.c:20:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: -fcontracts-prove-fold written explicitly, after
// -fno-contracts-prove-fold, turns folding back on.
//
// Mirror: clang/test/Contracts/p4405-fold-explicit.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fno-contracts-prove-fold -fcontracts-prove-fold -fcontract-evaluation-semantic=prove" }

void f ()
{
  contract_assert (true);	// { dg-bogus "cannot be proven|always false" }
  contract_assert (false);	// { dg-error "contract predicate is always false" }
}
