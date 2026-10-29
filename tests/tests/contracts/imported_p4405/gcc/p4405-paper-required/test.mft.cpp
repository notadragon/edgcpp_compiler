//remark: imported from gcc:p4405-paper-required.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-paper-required.json
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_configuration_file=p4405-paper-required.json -O2
//match_regex: Test_name.c:29:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Proving Preconditions": a configuration entry with the analysis
// key gives the analysis evaluations of the "required" group's
// preconditions the prove semantic, at each call; f(0)'s predicate is
// always false once the argument is substituted, and f(1) is proven.  The
// entry says nothing about the runtime evaluations.  The paper's example,
// with its comments, and its configuration
// (p4405-paper-required-noconfig.C is the same without it).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-paper-required.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>  // `""group`

void f(int x) pre<"required"group>(x > 0);	// { dg-message "the predicate of the contract assertion" }

void g()
{
  f(1);  // OK	{ dg-bogus "always false|cannot be proven" }
  f(0);  // Error if the `"required"` group has the *prove* semantic	{ dg-error "contract predicate is always false" }
}
