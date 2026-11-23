//remark: imported from gcc:p4451-precedes-p3400.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 25: (?:catastrophic )?error
//match_regex: ", line 27: (?:catastrophic )?error
// p4451-precedes.C under -fcontracts-p3400 alone
// (the P3400 label): the P4451 checks step aside and the specifier is
// a label expression.  What P4451 rejects is rejected by P3400's own
// checks, or accepted where P3400 finds a label (<contracts>'s
// contract-control using-directive makes an unqualified `enforced'
// find the standard one).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cassert>

void f (int x) pre<enforced> (x > 0);	// { dg-error "(was not declared in this scope|has not been declared)" }
struct S { void m (int x) pre<std::contracts::controls::enforced> (x > 0); };	// { dg-error "(was not declared in this scope|has not been declared)" }
void g (int x)
{
  contract_assert<std::contracts::controls::enforced> (x > 0);	// { dg-error "(was not declared in this scope|has not been declared)" }
}
void f2 (int x) pre<::std::contracts::controls::enforced> (x > 0);	// { dg-error "(was not declared in this scope|has not been declared)" }

#include <contracts>

void h (int x) pre<std::contracts::controls::enforced> (x > 0);
struct T { void m (int x) pre<std::contracts::controls::enforced> (x > 0); };
void k (int x)
{
  using namespace std::contracts::controls;
  contract_assert<enforced> (x > 0);
}
