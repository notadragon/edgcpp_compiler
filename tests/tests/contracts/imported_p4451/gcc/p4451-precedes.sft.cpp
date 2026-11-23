//remark: imported from gcc:p4451-precedes.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451
//match_regex: ", line 21: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 25: (?:catastrophic )?error
//match_regex: ", line 27: (?:catastrophic )?error
// P4451 [basic.contract.control] (D4451R1): name lookup for the
// id-expression must find the standard library declaration of
// std::contracts::controls::enforced, so none precedes it before
// <contracts> is included -- whether the name is unqualified or qualified.
// <cassert> does not declare it; <contracts> does, from where it is
// included on.  Under -fcontracts-p4451 alone.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cassert>

void f (int x) pre<enforced> (x > 0);	// { dg-error "assertion-control-specifier must name .std::contracts::controls::enforced." }
struct S { void m (int x) pre<std::contracts::controls::enforced> (x > 0); };	// { dg-error "must name .std::contracts::controls::enforced." }
void g (int x)
{
  contract_assert<std::contracts::controls::enforced> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }
}
void f2 (int x) pre<::std::contracts::controls::enforced> (x > 0);	// { dg-error "must name .std::contracts::controls::enforced." }

#include <contracts>

void h (int x) pre<std::contracts::controls::enforced> (x > 0);
struct T { void m (int x) pre<std::contracts::controls::enforced> (x > 0); };
void k (int x)
{
  using namespace std::contracts::controls;
  contract_assert<enforced> (x > 0);
}
