//remark: imported from gcc:lambda-modifies-param-in-pre.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 17: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
//match_regex: ", line 21: (?:catastrophic )?error
// A by-reference lambda capture in a predicate names the captured entity, so
// it is const there ([expr.prim.id.unqual]): the lambda cannot modify a
// parameter or the result name, in a free function's contract too.
//
// Mirror: clang/test/Contracts/lambda-modifies-param-in-pre.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

void f (int i) pre ([&] { return ++i > 0; } ()) {}                 // { dg-error "read-only" }

void f2 (int i) { contract_assert ([&] { return ++i > 0; } ()); }  // { dg-error "read-only" }

struct S { void m (int i) pre ([&] { return ++i > 0; } ()); };      // { dg-error "read-only" }
