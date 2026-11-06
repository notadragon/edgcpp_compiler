//remark: imported from gcc:p4405-group.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_group_evaluation_semantic=g:prove,h:diagnose --contract_evaluation_semantic=ignore
//match_regex: Test_name.c:27:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: prove and diagnose selected by -fcontract-group-evaluation-semantic=
// for the groups of an assertion-control label; a contract in neither group
// keeps the command-line semantic.
//
// Mirror: clang/test/Contracts/p4405-group.cpp in the llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -fcontract-group-evaluation-semantic=g:prove,h:diagnose -fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

int f (int x) pre<"g"group> (x > 0) { return x; }	// { dg-message "the predicate of the contract assertion" }
int g (int x) pre<"h"group> (x > 0) { return x; }	// { dg-message "the predicate of the contract assertion" }
int h (int x) pre (x > 0) { return x; }			// { dg-bogus "cannot be proven" }

int use (int y)
{
  return f (y)		// { dg-error "contract assertion cannot be proven" }
	 + g (y)	// { dg-warning "contract assertion cannot be proven" }
	 + h (y);	// { dg-bogus "cannot be proven" }
}
