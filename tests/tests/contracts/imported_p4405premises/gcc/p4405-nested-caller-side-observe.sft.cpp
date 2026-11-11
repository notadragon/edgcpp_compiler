//remark: imported from gcc:p4405-nested-caller-side-observe.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=observe --no_contracts_prove_callee_premises --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:34:[0-9]+: error: 
//match_regex: Test_name.c:42:[0-9]+: error: 
//match_regex: Test_name.c:56:[0-9]+: error: 
//match_regex: Test_name.c:63:1: error: 
//match_regex: Test_name.c:63:(?!1:)[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// EDG: adapted -- the nested precondition's diagnostic at its call in the
// check, on the line after the assertion's in the original, is reported on
// the assertion's line: the C++-generating back end prints the statement
// on one line, under one #line directive.
// P4405, DECISIONS.md E169: p4405-nested-caller-side.C with runtime observe,
// -O2, no premise copies: as under enforce, but g's observe check is no
// premise, so `checked_outer's evaluation at the call in the check is reported.
//
// Mirror: clang/test/Contracts/p4405-nested-caller-side.cpp in the
// llvm_llvm-project fork (its -O2 observe run).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=observe -fno-contracts-prove-callee-premises" }
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

// E169 (7): see p4405-nested-caller-side.C.
int
checked_outer (int y)
{
  contract_assert (		// { dg-error "contract assertion cannot be proven" }
    g (y) == 1);		// { dg-error "contract assertion cannot be proven" }
  return y;
}
