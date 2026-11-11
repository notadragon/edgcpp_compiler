//remark: imported from gcc:p4405-shadow-O0-default.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove -O0
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405 stage 3, D4405's proof optimization level (DECISIONS.md E162,
// E166): at -O0 the default proof level is 2, and the proof shadows are
// optimized at that level although the code is not, so p4405-shadow-O0.C's
// evaluations are proven as at -O2.  The code is the same as without the
// shadows and with ignore; -fcontracts-prove-assume, on by default, assumes
// nothing at -O0, where the code drops every assumption that is not trivial
// (DECISIONS.md E167).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0" }
// { dg-additional-options "-fcontract-evaluation-semantic=prove" }

int f (int i, int n)
{
  if (i < n)
    contract_assert (i < n);	// { dg-bogus "(cannot be proven|always false)" }
  contract_assert (2 > 1);	// { dg-bogus "(cannot be proven|always false)" }
  int c = n < 0 ? 0 : 1;
  contract_assert (c >= 0);	// { dg-bogus "(cannot be proven|always false)" }
  return c;
}

//
