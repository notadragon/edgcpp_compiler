//remark: imported from gcc:p4405-override-labels.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p3400 --contracts_p4405 -O2
//match_regex: Test_name.c:39:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions": each override obligation is an analysis
// evaluation of the overrider's precondition, with that precondition's
// own analysis semantic: here the build's is ignore, so only a
// precondition labeled always_prove is analyzed in the override shadow.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-additional-options "" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using enum evaluation_semantic;

template<evaluation_semantic S>
struct always {
  using assertion_control_object = always;
  static constexpr evaluation_semantic_set allowed_semantics{S};
  constexpr evaluation_semantic compute_semantic (evaluation_semantic) const
  { return S; }
};
constexpr always<prove> always_prove{};

struct B {
  virtual int f (int x) pre (x > 0);
  virtual int g (int x) pre (x > 0);
};

struct D : B {
  int f (int x) override	// { dg-error "cannot be proven for a call through 'virtual int B::f\\(int\\)'" }
    pre<always_prove> (x > 1)	// { dg-message "the predicate of the contract assertion" }
    pre (x > 2)			// not analyzed: ignore
  { return x; }
  int g (int x) override	// { dg-bogus "cannot be proven" }
    pre<always_prove> (x >= 1)
  { return x; }
};

void use () { D d; }

// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
