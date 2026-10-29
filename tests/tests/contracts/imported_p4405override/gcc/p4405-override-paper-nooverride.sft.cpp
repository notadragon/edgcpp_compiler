//remark: imported from gcc:p4405-override-paper-nooverride.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --no_contracts_prove_override --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:31:[0-9]+: error: 
//match_regex: Test_name.c:40:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions", without override shadows
// (-fno-contracts-prove-override, DECISIONS.md E157): each override
// obligation is one of the overrider itself, with no premise from the
// overridden function.  D::f's precondition `x >= 0`, proven in its
// override shadow from B::f's `x > 0` (p4405-override-paper.C), must now
// be a tautology, and is diagnosed; B::f's postcondition `r > 0` is an
// obligation in D::f's ordinary shadow, proven there from D::f's own
// precondition, enforced at entry, and in E::f's from E::f's.  D::f's own
// postcondition needs nothing beyond its analysis evaluation.  No override
// shadow is made.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fno-contracts-prove-override -fcontract-analysis-semantic=prove -O2" }
// { dg-additional-options "" }

struct B {
  virtual int f(int x)
    pre(x > 0)
    post(r: r > 0);
};

struct D : B {
  int f(const int x) override	// { dg-error "contract assertion cannot be proven for a call through 'virtual int B::f\\(int\\)'" }
    pre(x >= 0)			// { dg-message "the predicate of the contract assertion" }
    post(r: r > x)
  {
    return x + 1;
  }
};

struct E : B {
  int f(int x) override		// { dg-error "contract assertion cannot be proven for a call through 'virtual int B::f\\(int\\)'" }
    pre(x > 10)			// { dg-message "the predicate of the contract assertion" }
  {
    return x;
  }
};

B *make (bool d) { return d ? static_cast<B *> (new D) : new E; }

// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
