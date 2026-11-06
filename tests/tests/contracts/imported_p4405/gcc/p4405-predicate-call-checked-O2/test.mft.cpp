//remark: imported from gcc:p4405-predicate-call-checked-O2.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-predicate-call-checked.json
//options: --c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-predicate-call-checked.json -O2
//match_regex: Test_name.c:30:[0-9]+: error: 
//match_regex: Test_name.c:38:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: p4405-predicate-call-checked.C at -O2, where the proof shadow
// decides the analysis evaluations at the calls in the checks (GCC-953):
// none in h, whose checks evaluate check (v) only after `v > 5', which
// folding cannot use but the shadow can; and the one in g's assertion is
// proven (conditionally) by the one in g's precondition check before it,
// whose marker is a premise for what follows, as for any two calls of
// check in turn.
//
// Mirror: clang/test/Contracts/p4405-predicate-call-checked.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-predicate-call-checked.json" }

bool check (int v)
  pre (v > 0);			// { dg-message "the predicate of the contract assertion" }
				// { dg-message "precondition of a call in the predicate cannot be proven" "" { target *-*-* } .-1 }

void g (int v)
  pre (check (v))		// { dg-message "the predicate of the contract assertion" }
				// { dg-error "contract assertion cannot be proven" "" { target *-*-* } .-1 }
{
  contract_assert (check (v));	// { dg-bogus "cannot be proven" }
}

void use (int v)
{
  g (v);			// { dg-error "contract assertion cannot be proven" }
}

void h (int v)
  pre (v > 5 && check (v))	// { dg-bogus "cannot be proven" }
{
  contract_assert (v > 5 && check (v));	// { dg-bogus "cannot be proven" }
}
