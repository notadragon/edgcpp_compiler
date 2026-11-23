//remark: imported from gcc:p4405-shadow-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contracts_prove_optimize=0 --contract_evaluation_semantic=prove -O0
//match_regex: Test_name.c:24:[0-9]+: error: 
//match_regex: Test_name.c:27:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405 stage 3: at proof level 0 there is no proof shadow, so folding is
// final: the evaluations a shadow proves (at -O0 too, at the default
// proof level: p4405-shadow-O0-default.C) are diagnosed, each once, and
// one that folds is still proven.
//
// Mirror: clang/test/Contracts/p4405-shadow-O0.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0 -fcontracts-prove-optimize=0" }
// { dg-additional-options "-fcontract-evaluation-semantic=prove" }

int f (int i, int n)
{
  if (i < n)
    contract_assert (i < n);	// { dg-error "contract assertion cannot be proven" }
  contract_assert (2 > 1);	// { dg-bogus "(cannot be proven|always false)" }
  int c = n < 0 ? 0 : 1;
  contract_assert (c >= 0);	// { dg-error "contract assertion cannot be proven" }
  return c;
}
