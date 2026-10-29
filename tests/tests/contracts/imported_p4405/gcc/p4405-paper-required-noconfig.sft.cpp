//remark: imported from gcc:p4405-paper-required-noconfig.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405R0, "Proving Preconditions": p4405-paper-required.C without its
// configuration -- the analysis evaluations are ignore, and nothing is
// diagnosed.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>  // `""group`

void f(int x) pre<"required"group>(x > 0);

void g()
{
  f(1);  // OK	{ dg-bogus "always false|cannot be proven" }
  f(0);  // Error if the `"required"` group has the *prove* semantic	{ dg-bogus "always false|cannot be proven" }
}
