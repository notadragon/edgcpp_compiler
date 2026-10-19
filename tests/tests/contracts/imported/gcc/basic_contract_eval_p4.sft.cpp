//remark: imported from gcc:basic.contract.eval.p4.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// N5008 :
// basic.contract.eval/p4
// The evaluation of a contract assertion using the ignore semantic has no effect.
// [Note 2 : The predicate is potentially evaluated (6.3), but not evaluated. — end note]
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

int f(int i, int j = 1)
  pre (i > 0)
  post (r: r < 5)
{
  contract_assert ( j > 0);
  return i;
}

int main(int, char**)
{
  f (0,0);
}

void
handle_contract_violation (const std::contracts::contract_violation &)
{
  __builtin_abort ();
}
