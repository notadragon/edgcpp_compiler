//remark: imported from gcc:p4405-override-param.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:30:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions", DECISIONS.md E153: in an override shadow,
// a parameter the overridden function's postcondition names is what the
// P3097 wrapper evaluates the postcondition with.  A parameter received by
// value is the wrapper's own copy, which the overrider cannot modify, so
// the shadow takes a snapshot of it at entry; one received by invisible
// reference (a class with a nontrivial copy constructor) is the very
// object the overrider modifies.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-analysis-semantic=prove -O2" }

struct S { S (const S &); int v; };

struct B {
  virtual int f (const int x) post (r: r == x);
  virtual int g (const int x) post (r: r == x);	// { dg-message "the predicate of the contract assertion" }
  virtual int h (const S s) post (r: r == s.v);
};

struct D : B {
  // The wrapper's x is the value at entry.
  int f (int x) override { int r = x; x = 0; return r; }	// { dg-bogus "cannot be proven" }
  int g (int x) override			// { dg-error "cannot be proven for a call through 'virtual int B::g\\(int\\)'" }
  { x = 5; return x; }
  // The wrapper's s is D::h's.
  int h (S s) override { s.v = 3; return 3; }	// { dg-bogus "cannot be proven" }
};

void use () { D d; }
