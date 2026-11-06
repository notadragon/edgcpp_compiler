//remark: imported from gcc:p4405-nested-caller-side.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=enforce --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:34:[0-9]+: error: 
//match_regex: Test_name.c:42:[0-9]+: error: 
//match_regex: Test_name.c:56:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405, DECISIONS.md E169: a caller-side precondition's analysis
// evaluation at a call depends on the preconditions of the calls in its
// predicate, with the call's arguments for the callee's parameters: `f (0)'
// makes g's precondition always false.  Here the predicate is also checked
// at run time on the callee's side (enforce), where g's precondition is
// analyzed at its call in f's check.  The shape crashed Clang's proof
// shadow at -O1/-O2 (llvm_llvm-project, PC2-Clang), which GCC never did.
// Runtime ignore: p4405-nested-caller-side-ignore.C; -O0:
// p4405-nested-caller-side-O0.C.
//
// Mirror: clang/test/Contracts/p4405-nested-caller-side.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O2" }
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
  return f (y);			// { dg-bogus "cannot be proven" }
}

int
caller_zero ()
{
  return f (0);			// { dg-error "contract assertion cannot be proven" }
}
