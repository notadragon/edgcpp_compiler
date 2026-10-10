//remark: imported from gcc:fno-contracts-subflag.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++23 --contracts_p3400 --no_contracts_p3400
//match_regex: ", line 18: (?:catastrophic )?error
// A per-paper contracts sub-flag that a later -fno-contracts-pNNNN cancels
// leaves contracts off.
//
// -std=c++23 is essential: at C++26 contracts are on regardless of the
// sub-flags.
//
// Mirror: clang/test/Contracts/fno-contracts-subflag-driver.cpp in the
// llvm_llvm-project fork.

// { dg-do compile }
// { dg-additional-options "-std=c++23 -fcontracts-p3400 -fno-contracts-p3400" }

int f (int x) pre (x > 0) { return x; } // { dg-error "expected initializer before .pre." }
