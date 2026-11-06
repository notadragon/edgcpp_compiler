//remark: imported from gcc:p4451-precedes-both.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400 --contracts_p4451
//match_regex: ", line 18: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
// P4451 [basic.contract.control]: a declaration of
// std::contracts::labels::terminating shall precede each
// assertion-control-specifier.  <cassert> does not declare it; <contracts>
// does, from where it is included on.  Under -fcontracts-p3400 and -fcontracts-p4451 (the P3400 label).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cassert>

void f (int x) pre<terminating> (x > 0);	// { dg-error "'terminating' was not declared in this scope" }
struct S { void m (int x) pre<terminating> (x > 0); };	// { dg-error "'terminating' was not declared in this scope" }
void g (int x)
{
  contract_assert<terminating> (x > 0);	// { dg-error "'terminating' was not declared in this scope" }
}

#include <contracts>

void h (int x) pre<terminating> (x > 0);
struct T { void m (int x) pre<terminating> (x > 0); };
void k (int x)
{
  contract_assert<terminating> (x > 0);
}
