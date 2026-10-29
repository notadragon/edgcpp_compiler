//remark: imported from gcc:p4405-werror.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=diagnose -Werror=contract-unproven
//match_regex: Test_name.c:19:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: -Werror=contract-unproven makes the diagnose semantic's warning an
// error.
//
// Mirror: clang/test/Contracts/p4405-werror.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=diagnose -Werror=contract-unproven" }
// { dg-prune-output "some warnings being treated as errors" }

int f (int x) pre (x > 0) { return x; }	// { dg-message "the predicate of the contract assertion" }
int g (int y) { return f (y); }		// { dg-error "contract assertion cannot be proven \\\[-Werror=contract-unproven\\\]" }
