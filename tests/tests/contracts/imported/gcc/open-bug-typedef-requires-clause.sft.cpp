//remark: imported from gcc:open-bug-typedef-requires-clause.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26
//match_regex: ", line 16: (?:catastrophic )?error
// Mirror of an open Clang bug (CLANG-39 in that fork's bug-reports/; stock
// Clang has it, no contracts involved): a requires-clause may only appear on
// a declarator that declares a templated function ([dcl.decl.general]).
// Clang accepts `typedef int F (int) requires true;'; GCC diagnoses it, and
// this pins that.
//
// Mirror: clang/test/Contracts/OpenBugs/typedef-requires-clause.cpp in the
// llvm_llvm-project fork, which is XFAIL.
// { dg-do compile { target c++20 } }

typedef int F (int) requires true; // { dg-error "requires-clause on typedef" }
