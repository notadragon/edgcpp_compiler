//remark: imported from gcc:p4405-prove-optimize-follow-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contracts_prove_optimize=follow --contract_evaluation_semantic=prove -O0
//match_regex: Test_name.c:21:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E205):
// -fcontracts-prove-optimize=follow at -O0 is level 0, folding only, so a
// function optimized at -O2 by its own attribute gets no proof shadow
// either: the attribute does not alter the proofs (E168 (7)).  At the
// default level, 2, it is proven (p4405-prove-optimize-default-O0.C).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0 -fcontracts-prove-optimize=follow" }
// { dg-additional-options "-fcontract-evaluation-semantic=prove" }

[[gnu::optimize ("O2")]] int f (int i, int n)
{
  if (i < n)
    contract_assert (i < n);	// { dg-error "contract assertion cannot be proven" }
  return i;
}
