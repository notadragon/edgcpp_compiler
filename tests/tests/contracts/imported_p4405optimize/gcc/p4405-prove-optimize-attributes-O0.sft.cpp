//remark: imported from gcc:p4405-prove-optimize-attributes-O0.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_evaluation_semantic=prove -O0
//match_regex: Test_name.c:59:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E165): a -O0
// build proving at the default level, 2, gives its proofs what the level
// gives an -O2 build and -O0 does not -- type-based alias analysis (h:
// storing a float cannot change an int), the range of a bool load (r) and
// inlining (k: an inline callee; s: an always_inline one) -- and the flags
// the user gave still apply, as at -O2 (E165): -fno-strict-aliasing loses
// h, -fno-inline loses k, n and o (s is always_inline), while
// -fno-inline-functions, -fno-inline-small-functions and -finline-functions
// change nothing here (the callees are declared inline).  A callee's own
// noinline is followed: m is not inlined, and nothing else proves its
// assertion; its own optimize ("O0") is not (n, E168 (7)), nor does its
// cold attribute keep it out of line (o).  At proof level 0 only folding
//
// compares the assertions that are not proven, by the tags on their lines,
// with those expected.
//
// Mirror: clang/test/Contracts/p4405-prove-optimize-attributes-O0.cpp in
// the llvm_llvm-project fork (Clang: optnone, its m, keeps the callee out
// of line; GCC's m is noinline, and a noinline callee's range would be
// known through IPA, so its assertion needs more than a range).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-evaluation-semantic=prove -O0" }

int h (int *ip, float *fp)
{
  *ip = 1;
  *fp = 1.0f;
  contract_assert (*ip == 1);	// [h] { dg-bogus "cannot be proven" }
  return 0;
}

int r (bool *b)
{
  bool x = *b;
  contract_assert (x == 0 || x == 1);	// [r] { dg-bogus "cannot be proven" }
  return x;
}

inline int add1 (int v) { return v + 1; }
int k (int v)
{
  int c = add1 (v);
  contract_assert (c != v);	// [k] { dg-bogus "cannot be proven" }
  return c;
}

[[gnu::noinline]] inline int add2 (int v) { return v + 1; }
int m (int v)
{
  int c = add2 (v);
  contract_assert (c != v);	// [m] { dg-error "contract assertion cannot be proven" }
  return c;
}

[[gnu::optimize ("O0")]] inline int add3 (int v) { return v + 1; }
int n (int v)
{
  int c = add3 (v);
  contract_assert (c != v);	// [n] { dg-bogus "cannot be proven" }
  return c;
}

[[gnu::cold]] inline int add4 (int v) { return v + 1; }
int o (int v)
{
  int c = add4 (v);
  contract_assert (c != v);	// [o] { dg-bogus "cannot be proven" }
  return c;
}

[[gnu::always_inline]] inline int add5 (int v) { return v + 1; }
int s (int v)
{
  int c = add5 (v);
  contract_assert (c != v);	// [s] { dg-bogus "cannot be proven" }
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
