//remark: imported from gcc:p4405-template.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove
//match_regex: Test_name.c:27:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: a contract of a template is diagnosed once per instantiation; a
// template that is never instantiated is not diagnosed.  D4405: a
// precondition is diagnosed at each call, so here once per instantiation
// of the template that calls the function.
//
// Mirror: clang/test/Contracts/p4405-template.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove" }

template<typename T>
T f (T x)
  pre (x > 0)	// { dg-message "the predicate of the contract assertion" }
{ return x; }

template<typename T>
T call (T y)
{
  return f (y);	// { dg-error "contract assertion cannot be proven" }
}

template<typename T>
T never (T x)
  pre (x > 0)	// { dg-bogus "cannot be proven" }
{ return x; }

// { dg-message "T = int" "" { target *-*-* } 0 }
// { dg-message "T = long" "" { target *-*-* } 0 }

template<typename T>
T never_call (T y)
{
  return never (y);	// { dg-bogus "cannot be proven" }
}

int g (int y)
{
  int a = call (y);		// { dg-message "required from here" }
  long b = call ((long) y);	// { dg-message "required from here" }
  return a + (int) b;
}
