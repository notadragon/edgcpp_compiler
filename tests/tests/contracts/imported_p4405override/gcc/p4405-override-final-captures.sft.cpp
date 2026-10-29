//remark: imported from gcc:p4405-override-final-captures.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p3098 --contracts_p4405 --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:30:[0-9]+: error: 
//match_regex: Test_name.c:42:[0-9]+: error: 
//match_regex: Test_name.c:43:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions": `final' changes nothing about an
// overrider's own override shadows (it only stops later overriders), and a
// postcondition with captures (P3098), of the overridden function or of the
// overrider, is decided at once in every override shadow, as its own
// analysis evaluation is: its captures are initialized only for a checked
// evaluation, so only folding can settle it.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p3098 -fcontracts-p4405 -fcontract-analysis-semantic=prove -O2" }
// { dg-additional-options "" }

int cap (int);

struct B {
  virtual int f (int x) post [c = cap (x)] (r: r >= c);	// { dg-message "the predicate of the contract assertion" }
  virtual int g (int x) pre (x > 0);
  virtual int h (int x) pre (x > 0) post [c = 1] (r: r >= c);	// { dg-message "the predicate of the contract assertion" }
};

struct D final : B {
  int f (int x) override	// { dg-error "cannot be proven for a call through 'virtual int B::f\\(int\\)'" }
  { return cap (x); }
  int g (int x) override final	// { dg-bogus "cannot be proven" }
    pre (x > -1)
  { return x; }
};

struct E : B {
  // Both postconditions have captures: E::h's own analysis evaluation
  // (an error under prove), and in the override shadow E::h's and B::h's
  // obligations, are each decided at once, unproven, although B::h's
  // precondition would give `r >= 1'.
  int h (int x) override	// { dg-error "cannot be proven for a call through 'virtual int B::h\\(int\\)'" }
    post [d = x] (r: r == d)	// { dg-error "contract assertion cannot be proven" }
  { return x; }			// { dg-message "the predicate of the contract assertion" "" { target *-*-* } .-1 }
};

void use () { D d; E e; }

// D::g's precondition, proven from B::g's in its override shadow, is the
// only deferred obligation.
// (EDG: a dg-final scan of the original, dropped)
