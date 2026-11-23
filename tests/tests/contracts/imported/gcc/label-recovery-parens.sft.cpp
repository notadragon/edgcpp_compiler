//remark: imported from gcc:label-recovery-parens.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --no_contracts_p4451
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 23: (?:catastrophic )?error
// Without -fcontracts-p3400 (and -fcontracts-p4451, which C++26 enables and
// which gives such a specifier its own meaning) an assertion-control label is
// diagnosed and its <...> skipped; a '>' inside parentheses does not end the
// skip, so the one error is not followed by a cascade.
//
// Mirror: clang/test/Contracts/label-recovery-parens.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fno-contracts-p4451" }

struct L { using assertion_control_object = L; };
constexpr L l{};
constexpr L pick (bool) { return l; }

int f (int x) pre<pick (1 > 0)> (x > 0); // { dg-error "assertion-control labels require" }
// { dg-bogus "expected" "" { target *-*-* } .-1 }
int g (int x) pre<l> (x > 0); // { dg-error "assertion-control labels require" }
