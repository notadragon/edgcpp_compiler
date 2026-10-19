//remark: imported from gcc:generic-lambda-modifies-param.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 17: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
// A generic lambda's by-reference capture in a contract names the captured
// entity, which is const there ([expr.prim.id.unqual]), so modifying a
// parameter or the result name through it is ill-formed.
//
// Mirror: clang/test/Contracts/generic-lambda-modifies-param.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -Wno-template-body" }
// { dg-prune-output "instantiating erroneous template" }

void f (int i) pre ([&] (auto k) { return ++i > k; } (0)) {}	// { dg-error "read-only" }
void use () { f (1); }
int g () post (r: [&] (auto k) { return ++r > k; } (0)) { return 1; } // { dg-error "read-only" }
