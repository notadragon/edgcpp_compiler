//remark: imported from gcc:contracts-config-without-contracts.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options_sep: !
//options: --c++23 --contract_group_evaluation_semantic=a:bogus
// With contracts off (C++23, no -fcontracts) the contract configuration
// options are not used, so they are not parsed either: say so rather than
// accept them silently.  Clang warns that the argument is unused.
//
// Mirror (C, no -fcontracts-p4299): gcc.dg/contracts/contracts-config-without-p4299.c
// { dg-do compile }
// { dg-options "-std=c++23 -fcontract-group-evaluation-semantic=a:bogus" }
// { dg-warning "contract configuration options have no effect without .-fcontracts." "" { target *-*-* } 0 }

int x;
