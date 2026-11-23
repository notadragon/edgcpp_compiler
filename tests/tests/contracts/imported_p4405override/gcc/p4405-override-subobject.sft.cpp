//remark: imported from gcc:p4405-override-subobject.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_analysis_semantic=prove --contract_evaluation_semantic=observe -O2
//match_regex: Test_name.c:33:[0-9]+: error: 
//match_regex: Test_name.c:57:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions", DECISIONS.md E154: when a base class
// occurs as more than one subobject, an overrider gets one override shadow
// per subobject if the overridden function's contract assertions name
// `this' -- they then read a different object in each -- and one per
// overridden function otherwise.  A virtual base is one subobject however
// often it is reached.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-analysis-semantic=prove -fcontract-evaluation-semantic=observe -O2" }
// { dg-additional-options "" }

struct B {
  int m;
  virtual int f () pre (m > 0) post (r: r == m);	// { dg-message "the predicate of the contract assertion" }
  virtual int g (int x) pre (x > 0) post (r: r > 0);
  virtual int h ();
};

struct C1 : B {};
struct C2 : B {};

struct D : C1, C2 {
  // Proven for the B in C1, not for the B in C2: two override shadows,
  // one error.
  int f () override		// { dg-error "cannot be proven for a call through 'virtual int B::f\\(\\)'" }
  { return C1::m; }
  // Nothing names `this': one override shadow.
  int g (int x) override { return x; }
  // B::h has no contract assertions, so nothing names `this': one pair,
  // whose only obligation, D::h's precondition (DECISIONS.md E156), is
  // settled by folding.
  int h () override pre (true) { return 0; }
};

// A virtual base reached twice is one subobject.
struct V1 : virtual B {};
struct V2 : virtual B {};

struct W : V1, V2 {
  int f () override { return m; }	// { dg-bogus "cannot be proven" }
  int g (int x) override { return x; }
};

// A base class occurring as a non-virtual and a virtual base: two pairs,
// both unproven -- the non-virtual one's rightly; the virtual one's because
// its two conversions of `this' read the virtual base's offset from
// different vtable pointers, which the optimizers cannot equate.
struct X : C1, V1 {
  int f () override		// { dg-error "cannot be proven for a call through 'virtual int B::f\\(\\)'" }
  { return V1::m; }
};

void use () { D d; W w; X x; }

// D::f 2, D::g 1, W::f 1, W::g 1, X::f 2; D::h's precondition is settled
// by folding, so it has none.
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
