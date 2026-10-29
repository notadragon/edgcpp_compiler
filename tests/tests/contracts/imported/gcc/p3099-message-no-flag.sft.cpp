//remark: imported from gcc:p3099-message-no-flag.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 10: (?:catastrophic )?error
// P3099: Verify that message syntax is rejected without -fcontracts-p3099.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

void f(int x) pre(x > 0, "message") { }  // { dg-error "expected" }
