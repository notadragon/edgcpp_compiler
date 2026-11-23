//remark: imported from gcc:p4405-fold-umbrella.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3850 --contract_evaluation_semantic=prove --contracts_prove_optimize=0
//match_regex: Test_name.c:21:[0-9]+: error: 
//match_regex: Test_name.c:22:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: the umbrella -fcontracts-p3850 implies -fcontracts-p4405, and so
// the folding strategy.
//
// Mirror: clang/test/Contracts/p4405-fold-umbrella.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3850 -fcontract-evaluation-semantic=prove -fcontracts-prove-optimize=0" }

void f (int x)
{
  contract_assert (true);	// { dg-bogus "(cannot be proven|always false)" }
  contract_assert (false);	// { dg-error "contract predicate is always false" }
  contract_assert (x > 0);	// { dg-error "contract assertion cannot be proven" }
}
