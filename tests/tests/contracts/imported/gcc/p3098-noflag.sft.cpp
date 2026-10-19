//remark: imported from gcc:p3098-noflag.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 11: (?:catastrophic )?error
// P3098: Postcondition captures require -fcontracts-p3098.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

void f(int i)
  post [i] (true); // { dg-error "postcondition captures require" }
