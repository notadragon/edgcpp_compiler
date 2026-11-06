//remark: imported from gcc:p4405-diagnose.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=diagnose
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405: every evaluation with the diagnose semantic gets a
// -Wcontract-unproven warning, which is on by default -- a precondition (at
// each call, D4405), a postcondition and an assertion.
//
// Mirror: clang/test/Contracts/p4405-diagnose.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=diagnose" }

int f (int x)
  pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
  post (r: r > 0)		// { dg-warning "contract assertion cannot be proven" }
{
  contract_assert (x != 3);	// { dg-warning "contract assertion cannot be proven" }
  return x;
}

int g (int y)
{
  return f (y);			// { dg-warning "contract assertion cannot be proven \\\[-Wcontract-unproven\\\]" }
}
