//remark: imported from gcc:p4405-fold-template.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove --contracts_prove_optimize=0
//match_regex: Test_name.c:26:[0-9]+: error: 
//match_regex: Test_name.c:48:[0-9]+: error: 
//match_regex: Test_name.c:50:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405 stage 2: folding happens per template instantiation, after
// substitution, so one instantiation can be proven and another always false
// or unproven.  An uninstantiated template is not diagnosed.  A
// precondition is diagnosed at each call (D4405), after its substitution
// for the call's template arguments.
//
// Mirror: clang/test/Contracts/p4405-fold-template.cpp in the
// llvm_llvm-project fork (Clang does not fold x == x, so its mirror expects
// it unproven for every instantiation).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove -fcontracts-prove-optimize=0" }

template <int N>
void f ()
{
  contract_assert (N > 0);	// { dg-error "contract predicate is always false" }
}

template <class T>
T g (T x)
  pre (sizeof (T) >= 4)		// { dg-message "the predicate of the contract assertion" }
  pre (x == x)			// { dg-message "the predicate of the contract assertion" }
{
  return x;
}

template <class T>
void never_used (T)
{
  contract_assert (false);	// { dg-bogus "(always false|cannot be proven)" }
}

void use (double d)
{
  f<1> ();			// proven
  f<0> ();			// the "always false" above
  g (1);			// both proven
  g ('c');			// { dg-error "contract predicate is always false" "sizeof (char) is 1" }
  g (1.0);			// proven: 1.0 == 1.0
  g (d);			// { dg-error "contract assertion cannot be proven" "a double may be a NaN" }
}
