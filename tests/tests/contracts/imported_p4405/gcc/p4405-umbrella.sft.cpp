//remark: imported from gcc:p4405-umbrella.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3850 --contract_evaluation_semantic=prove
//match_regex: Test_name.c:25:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: the umbrella -fcontracts-p3850 implies -fcontracts-p4405, which
// implies -fcontracts.
//
// Mirror: clang/test/Contracts/p4405-umbrella.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3850 -fcontract-evaluation-semantic=prove" }

#ifndef __cpp_contracts_prove
#error "__cpp_contracts_prove not defined with -fcontracts-p3850"
#endif
#if __cpp_contracts_prove <= 0
#error "__cpp_contracts_prove not positive"
#endif

int f (int x) pre (x > 0) { return x; }	// { dg-message "the predicate of the contract assertion" }
int g (int y) { return f (y); }		// { dg-error "contract assertion cannot be proven" }
