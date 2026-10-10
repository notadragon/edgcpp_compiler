//remark: imported from gcc:label-recovery-parens.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 20: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
// Without -fcontracts-p3400 an assertion-control label is diagnosed and its
// <...> skipped; a '>' inside parentheses does not end the skip, so the one
// error is not followed by a cascade.
//
// Mirror: clang/test/Contracts/label-recovery-parens.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

struct L { using assertion_control_object = L; };
constexpr L l{};
constexpr L pick (bool) { return l; }

int f (int x) pre<pick (1 > 0)> (x > 0); // { dg-error "assertion-control labels require" }
// { dg-bogus "expected" "" { target *-*-* } .-1 }
int g (int x) pre<l> (x > 0); // { dg-error "assertion-control labels require" }
