//remark: imported from gcc:p4405-prove-optimize-ipa-O0.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=diagnose -O0:--c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=diagnose -O0 -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E166): in a
// -O0 build IPA pure-const, which is off at -O0, still runs for the
// functions optimized at the proof level -- here f's shadow -- and only for
// them: the real sq and f are not analyzed.  sq is never inlined; the
// level copy of it that the shadow calls is found const, so the two calls
// in the predicate are one, the assertion is proven, as at -O2, and the
// copy is gone before IPA.  The object code is the same as without the
// shadows.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0" }
// { dg-additional-options "-fcontract-evaluation-semantic=diagnose " }

[[gnu::noinline]] int sq (int x) { return x * x; }

void f (int n)
{
  contract_assert (sq (n) == sq (n));	// { dg-bogus "cannot be proven" }
}

//
//
//
//
//
