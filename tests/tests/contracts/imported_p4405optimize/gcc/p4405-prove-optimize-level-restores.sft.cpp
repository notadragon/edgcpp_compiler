//remark: imported from gcc:p4405-prove-optimize-level-restores.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove -O1
//match_regex: Test_name.c:66:[0-9]+: error: 
//match_regex: Test_name.c:77:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E165, E206),
// Clang's level-restores cases: the proofs are made at the proof level,
// with the flags the program was given, whatever the level and the
// function attributes it is compiled with.  In GCC the sums' facts are
// proven at every proof level from 1, and the counted loops' facts at
// none (GCC's loop optimizers do not bound c), so every build that has
// shadows -- -O1, -O3, -Os, -Oz, -Og, levels 1 and 3, with or without
// unrolling -- gives the same results: summ's own optimize ("Os") does not
// matter (E168 (7)), nor does sumc's cold attribute, which the shadow
// keeps; loop3 keeps its pragma.  A -O0 build proves at the proof level
// too, 2 by default, but makes no shadows at level 0 (`follow' there).
//
// compares the evaluations that are not proven, by the tags on their
// lines, with those expected.
//
// Mirror: clang/test/Contracts/p4405-prove-optimize-level-restores.cpp in
// the llvm_llvm-project fork (Clang: sum not proven at level 1, loop2
// proven from level 2 where loops are unrolled).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove -O1" }

int sum (const int *a, unsigned n)
{
  int t = 0;
  for (unsigned j = 0; j < n; ++j)
    t += a[j];
  contract_assert (t != 1 || n > 0);	// [sum] { dg-bogus "cannot be proven" }
  return t;
}

[[gnu::optimize ("Os")]] int summ (const int *a, unsigned n)
{
  int t = 0;
  for (unsigned j = 0; j < n; ++j)
    t += a[j];
  contract_assert (t != 1 || n > 0);	// [summ] { dg-bogus "cannot be proven" }
  return t;
}

[[gnu::cold]] int sumc (const int *a, unsigned n)
{
  int t = 0;
  for (unsigned j = 0; j < n; ++j)
    t += a[j];
  contract_assert (t != 1 || n > 0);	// [sumc] { dg-bogus "cannot be proven" }
  return t;
}

bool cond (int);

int loop2 ()
{
  int c = 0;
  for (int i = 0; i < 8; ++i)
    if (cond (i))
      ++c;
  contract_assert (c <= 8);	// [loop2] { dg-error "contract assertion cannot be proven" }
  return c;
}

int loop3 ()
{
  int c = 0;
#pragma GCC unroll 0
  for (int i = 0; i < 8; ++i)
    if (cond (i))
      ++c;
  contract_assert (c <= 8);	// [loop3] { dg-error "contract assertion cannot be proven" }
  return c;
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
