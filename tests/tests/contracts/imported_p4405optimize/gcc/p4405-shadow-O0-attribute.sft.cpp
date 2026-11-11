//remark: imported from gcc:p4405-shadow-O0-attribute.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405 stage 3, D4405's proof optimization level (DECISIONS.md E168 (7)):
// a function's own optimize attribute does not alter the proofs.  f,
// optimized at -O0 by its attribute in a -O2 compilation, gets a proof
// shadow optimized at the proof level, as g does, and its evaluation is
// proven.  The shadow does not keep the attribute.
//
// Mirror: none (GCC's optimize attribute).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=prove " }

[[gnu::optimize ("O0")]] int f (int i, int n)
{
  if (i < n)
    contract_assert (i < n);	// { dg-bogus "(cannot be proven|always false)" }
  return i;
}

int g (int i, int n)
{
  if (i < n)
    contract_assert (i < n);	// { dg-bogus "(cannot be proven|always false)" }
  return i;
}

//
//
