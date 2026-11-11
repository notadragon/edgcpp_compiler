//remark: imported from gcc:p4405-nested-caller-side-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=enforce --contract_analysis_semantic=prove -O0 --contracts_prove_optimize=0
//match_regex: Test_name.c:28:[0-9]+: error: 
//match_regex: Test_name.c:36:[0-9]+: error: 
//match_regex: Test_name.c:44:[0-9]+: error: 
//match_regex: Test_name.c:50:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405, DECISIONS.md E169: p4405-nested-caller-side.C at proof level
// 0, folding alone: the guarded call is not proven either.
//
// Mirror: clang/test/Contracts/p4405-nested-caller-side.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0 -fcontracts-prove-optimize=0" }
// { dg-additional-options "-fcontract-evaluation-semantic=enforce" }
// { dg-additional-options "-fcontract-analysis-semantic=prove" }

int g (int x) pre (x != 0);	// { dg-message "precondition of a call in the predicate cannot be proven" }
				// { dg-message "precondition of a call in the predicate is always false" "" { target *-*-* } .-1 }
				// { dg-message "the predicate of the contract assertion" "" { target *-*-* } .-2 }
int g (int) { return 1; }

int f (int y)
  pre (g (y) == 1)		// { dg-error "contract assertion cannot be proven" }
{
  return y;
}

int
caller (int y)
{
  return f (y);			// { dg-error "contract assertion cannot be proven" }
}

int
caller_guarded (int y)
{
  if (y == 0)
    return 0;
  return f (y);			// { dg-error "contract assertion cannot be proven" }
}

int
caller_zero ()
{
  return f (0);			// { dg-error "contract assertion cannot be proven" }
}
