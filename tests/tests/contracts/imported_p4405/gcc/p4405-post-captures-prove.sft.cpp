//remark: imported from gcc:p4405-post-captures-prove.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contracts_p4405 --contract_evaluation_semantic=prove
//match_regex: Test_name.c:21:[0-9]+: error: 
//match_regex: Test_name.c:22:[0-9]+: error: 
//match_regex: Test_name.c:26:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: a postcondition with captures (P3098) under prove is an error and
// nothing about its captures is emitted, so there is no internal error.  A
// precondition is analyzed at each call (D4405): use's call of g.
//
// Mirror: clang/test/Contracts/p4405-post-captures-prove.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontracts-p4405 -fcontract-evaluation-semantic=prove" }

int cap (int x);
int f (int x) post [c = cap (x)] (c > 0) { return x; }	// { dg-error "contract assertion cannot be proven" }
int g (int x) pre (x > 0) post [c = cap (x)] (r: r > c) { return x; }
// { dg-error "contract assertion cannot be proven" "" { target *-*-* } .-1 }
// { dg-message "the predicate of the contract assertion" "" { target *-*-* } .-2 }

int use (int y) { return g (y); }	// { dg-error "contract assertion cannot be proven" }
