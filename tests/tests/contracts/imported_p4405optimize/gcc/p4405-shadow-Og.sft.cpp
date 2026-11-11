//remark: imported from gcc:p4405-shadow-Og.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove -Og
//match_regex: Test_name.c:27:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405 stage 3, D4405's proof optimization level (DECISIONS.md E162,
// E168): at -Og the proof shadow is optimized at the proof level, 2 (by
// default and with `follow'), not by -Og's own optimizations: a constant
// propagated from a local variable is proven, and so is a dominating
// condition, which needs the value-range passes -Og does not run.  What
// nothing establishes is not proven.
//
// Mirror: none (GCC's -Og).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -Og" }
// { dg-additional-options "-fcontract-evaluation-semantic=prove" }

int f (int x)
{
  int k = 4;
  contract_assert (k == 4);	// { dg-bogus "(cannot be proven|always false)" }
  if (x > 0)
    contract_assert (x > 0);	// { dg-bogus "(cannot be proven|always false)" }
  contract_assert (x > 0);	// { dg-error "contract assertion cannot be proven" }
  return x + k;
}
