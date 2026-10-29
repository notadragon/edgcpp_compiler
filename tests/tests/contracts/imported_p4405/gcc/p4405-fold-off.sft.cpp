//remark: imported from gcc:p4405-fold-off.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --no_contracts_prove_fold --contract_evaluation_semantic=prove
//match_regex: Test_name.c:23:[0-9]+: error: 
//match_regex: Test_name.c:24:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: -fno-contracts-prove-fold turns the folding strategy off, giving
// stage 1's behaviour back: every evaluation with prove is an error, "cannot
// be proven", even when its predicate is true or false.  The -fno- form
// wins whichever side of -fcontracts-p4405 it is written on
// (p4405-fold-off-first.C).
//
// Mirror: clang/test/Contracts/p4405-fold-off.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fno-contracts-prove-fold -fcontract-evaluation-semantic=prove" }

void f ()
{
  contract_assert (true);	// { dg-error "contract assertion cannot be proven" }
  contract_assert (false);	// { dg-error "contract assertion cannot be proven" }
}
