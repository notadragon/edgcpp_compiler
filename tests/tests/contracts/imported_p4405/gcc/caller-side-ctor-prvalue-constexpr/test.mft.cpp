//remark: imported from gcc:caller-side-ctor-prvalue-constexpr.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: open-bug-caller-side.json
//options: --c++26 --contracts --contract_configuration_file=open-bug-caller-side.json
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// Under P3595 caller-side checking (caller observe, callee ignore), the
// constant evaluation of a call of a constexpr constructor whose object is
// a prvalue goes through the caller-side wrapper, which initializes the
// temporary as the constructor would (GCC-921;
// caller-side-ctor-prvalue.C runs the same calls).
//
// No Clang mirror: Clang has no caller-side checking.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/open-bug-caller-side.json" }

struct T {
  int v;
  constexpr T () pre (true) : v (1) {}
  constexpr T (int x) pre (x > 0) : v (x) {}
};

struct H {
  T t;
  constexpr H () : t () {}
};

constexpr int
f ()
{
  T a = T ();
  return T ().v + T (2).v + T {}.v + a.v + H ().t.v;
}

static_assert (f () == 6);
constexpr T t = T (3);
static_assert (t.v == 3);
