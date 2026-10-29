//remark: imported from gcc:p4405-prove.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove
//match_regex: Test_name.c:27:[0-9]+: error: 
//match_regex: Test_name.c:29:[0-9]+: error: 
//match_regex: Test_name.c:35:[0-9]+: error: 
//match_regex: Test_name.c:36:[0-9]+: error: 
//match_regex: Test_name.c:50:[0-9]+: error: 
//match_regex: Test_name.c:57:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: every evaluation with the prove semantic is an error until
// something can be proven -- a precondition at each call (D4405), a
// postcondition and an assertion in the function body. 
// (the number expected) makes a repeated error fail the test: a seventh
// would end the compilation.
//
// Mirror: clang/test/Contracts/p4405-prove.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove" }

int f (int x)
  pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
  post (r: r > 0)		// { dg-error "contract assertion cannot be proven" }
{
  contract_assert (x != 3);	// { dg-error "contract assertion cannot be proven" }
  return x;
}

void g (int x)
{
  contract_assert (x > 0);	// { dg-error "contract assertion cannot be proven" }
  f (x);			// { dg-error "contract assertion cannot be proven" }
}

// A destructor's contract assertion is diagnosed once, not once per clone,
// and a constructor's precondition once per call.  A virtual base makes the
// complete and base clones two bodies, and gives the constructor an
// in-charge parameter its complete object clone, which the call calls, has
// not: the argument is still substituted for the parameter (GCC-917).
struct V {};
struct S : virtual V {
  int v;
  S (int x)
    pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
    : v (x) {}
  ~S () { contract_assert (v > 0); }	// { dg-error "contract assertion cannot be proven" }
};

S s (1);			// { dg-bogus "cannot be proven" }

void make (int y)
{
  S t (y);			// { dg-error "contract assertion cannot be proven" }
}
