//remark: imported from gcc:p4405-override-post.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_analysis_semantic=diagnose -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions": an overrider's postconditions are proven
// in each override shadow at each return, ahead of their own checks, with
// the overridden function's preconditions as premises, and again in the
// overrider's ordinary shadow, without them -- distinct analysis
// evaluations, each diagnosed on its own.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-analysis-semantic=diagnose -O2" }
// { dg-additional-options "" }

// Provable for a call through B, from B::f's precondition, but not for a
// direct call.
namespace through {
  struct B { virtual int f (int x) pre (x > 0); };
  struct D : B {
    int f (const int x) override	// { dg-bogus "for a call through" }
      post (r: r > 0)		// { dg-warning "contract assertion cannot be proven" }
    { return x; }
  };
  void use () { D d; }
}

// An obligation is never proven by its own check: D::f's enforced
// postcondition follows its analysis evaluations, so neither the override
// shadow (B::f has no contract assertions, so it gives no premise:
// DECISIONS.md E156) nor the ordinary shadow proves it.
namespace own {
  struct B { virtual int f (int x); };
  struct D : B {
    int f (const int x) override	// { dg-warning "contract assertion cannot be proven for a call through 'virtual int own::B::f\\(int\\)'" }
      post (r: r > 0)		// { dg-warning "contract assertion cannot be proven" }
    { return x; }		// { dg-message "the predicate of the contract assertion" "" { target *-*-* } .-1 }
  };
  void use () { D d; }
}

// B::f's postcondition is proven after D::f's enforced check of the same
// predicate, which comes first; D::f's postcondition itself is not.
namespace order {
  struct B { virtual int f (int x) post (r: r > 0); };
  struct D : B {
    int f (const int x) override	// { dg-warning "contract assertion cannot be proven for a call through 'virtual int order::B::f\\(int\\)'" }
      post (r: r > 0)		// { dg-warning "contract assertion cannot be proven" }
    { return x; }		// { dg-message "the predicate of the contract assertion" "" { target *-*-* } .-1 }
  };
  void use () { D d; }
}

// through: D::f's postcondition (line 16) proven in the override shadow;
// order: B::f's (line 39) proven in D::f's, relying on D::f's ordinary
// obligation (line 42), which is unproven, so conditionally.
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
