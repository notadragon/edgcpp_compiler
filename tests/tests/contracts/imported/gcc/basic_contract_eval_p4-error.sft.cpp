//remark: imported from gcc:basic.contract.eval.p4-error.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=ignore
//match_regex: ", line 16: (?:catastrophic )?error
//match_regex: ", line 18: (?:catastrophic )?error
// N5008 :
// basic.contract.eval/p4
// The evaluation of a contract assertion using the ignore semantic has no effect.
// [Note 2 : The predicate is potentially evaluated (6.3), but not evaluated. — end note]
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=ignore" }

struct S;
int f(int i, int j = 1)
  pre (check(i)) // { dg-error "was not declared in this scope" }
{
  contract_assert ( S{}.member ); // { dg-error "invalid use of incomplete type" }
  return i;
}
