//remark: imported from gcc:p4405-flag.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=diagnose
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405: -fcontracts-p4405 defines __cpp_contracts_prove (checked defined
// and positive only) and accepts the prove and diagnose semantics.
//
// Mirror: clang/test/Contracts/p4405-flag.cpp in the llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=diagnose" }

#ifndef __cpp_contracts_prove
#error "__cpp_contracts_prove not defined with -fcontracts-p4405"
#endif
#if __cpp_contracts_prove <= 0
#error "__cpp_contracts_prove not positive"
#endif

int f (int x) pre (x > 0) { return x; }	// { dg-message "the predicate of the contract assertion" }
int g (int y) { return f (y); }		// { dg-warning "contract assertion cannot be proven" }
