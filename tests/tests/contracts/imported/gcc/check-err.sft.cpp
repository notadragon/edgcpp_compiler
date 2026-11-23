//remark: imported from gcc:cpp2a/check-err.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 12: (?:catastrophic )?error
//match_regex: ", line 14: (?:catastrophic )?error
//match_regex: ", line 21: (?:catastrophic )?error
// test that attribute syntax is no longer recognized
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fcontracts" }

int fun(int n)  [[ pre : n > 0 ]]; // { dg-error {expected ']' before ':' token} }
// { dg-warning {'pre' attribute directive ignored} "" { target *-*-* } .-1 }
int fun2(const int n)  [[ post : n > 0 ]]; // { dg-error {expected ']' before ':' token} }
// { dg-warning {'post' attribute directive ignored} "" { target *-*-* } .-1 }

int main()
{
  int x;

  [[assert: x >= 0]]; // { dg-error {expected ']' before ':' token} }
  // { dg-warning {attributes at the beginning of statement are ignored} "" { target *-*-* } .-1 }

  return 0;
}
