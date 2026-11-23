//remark: imported from gcc:p4298-semantic-without-flag.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=noexcept_enforce
// A P4298 noexcept semantic requested without -fcontracts-p4298 is
// downgraded to its plain counterpart, and the compiler says so.
//
// Clang: clang/test/Contracts/p4298-semantic-without-flag.cpp.
//
//
//
//
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=noexcept_enforce" }
// { dg-warning "requires .-fcontracts-p4298.; .enforce. is used instead" "" { target *-*-* } 0 }

int f (int x) pre (x > 0) { return x; }
