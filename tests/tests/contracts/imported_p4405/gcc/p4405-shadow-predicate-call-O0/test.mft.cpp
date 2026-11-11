//remark: imported from gcc:p4405-shadow-predicate-call-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-shadow-caller-side.json
//options: --c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-shadow-caller-side.json -O0
//match_regex: Test_name.c:30:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405 stage 3, DECISIONS.md E169: p4405-shadow-predicate-call.C at -O0,
// where no proof shadow is made: g's precondition still depends on
// check's, which is noted and not reported on its own.
//
// Mirror: clang/test/Contracts/p4405-shadow-predicate-call.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-shadow-caller-side.json" }

bool check (int v)
  pre (v > 0);			// { dg-message "precondition of a call in the predicate cannot be proven" }
				// { dg-bogus "the predicate" "" { target *-*-* } .-1 }

void g (int v)
  pre (check (v))		// { dg-message "the predicate of the contract assertion" }
{}

void use (int v)
{
  g (v);			// { dg-error "contract assertion cannot be proven" }
}
