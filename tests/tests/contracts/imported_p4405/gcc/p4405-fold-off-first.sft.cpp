//remark: imported from gcc:p4405-fold-off-first.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --no_contracts_prove_fold --contracts --contracts_p4405 --contract_evaluation_semantic=prove
//match_regex: Test_name.c:20:[0-9]+: error: 
//match_regex: Test_name.c:21:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: -fno-contracts-prove-fold written before -fcontracts-p4405 still
// turns folding off (p4405-fold-off.C).
//
// Mirror: clang/test/Contracts/p4405-fold-off-first.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fno-contracts-prove-fold -fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove" }

void f ()
{
  contract_assert (true);	// { dg-error "contract assertion cannot be proven" }
  contract_assert (false);	// { dg-error "contract assertion cannot be proven" }
}
