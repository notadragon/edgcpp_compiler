//remark: imported from gcc:p4405-shadow-callee-premises-post-captures.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contracts_p4405 --contract_evaluation_semantic=prove -O0:--c++26 --contracts --contracts_p3098 --contracts_p4405 --contract_evaluation_semantic=prove -O0 -O2
//match_regex: Test_name.c:23:[0-9]+: error: 
//match_regex: Test_name.c:27:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405's callee premise copies: g's premise copy, made at -O0 too at the
// default proof level, and the premise shadow calling it compile without
// an internal error though g's postcondition has a capture (P3098), which
// only a checked postcondition evaluates; the postcondition is not proven,
// nor is g's precondition at its call in use, at -O0 as at -O2.
//
// Mirror: clang/test/Contracts/p4405-shadow-callee-premises-post-captures.cpp
// in the llvm_llvm-project fork (CLANG-913, where the copy crashed).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontracts-p4405 -fcontract-evaluation-semantic=prove -O0" }
// { dg-additional-options "" }

int cap (int x);
int g (int x) pre (x > 0) post [c = cap (x)] (r : r > c) { return x; }
// { dg-error "contract assertion cannot be proven" "" { target *-*-* } .-1 }
// { dg-message "the predicate of the contract assertion" "" { target *-*-* } .-2 }

int use (int y) { return g (y); }	// { dg-error "contract assertion cannot be proven" }

//
//
//
