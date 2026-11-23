//remark: imported from gcc:p4405-override-nooverride.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --no_contracts_prove_override --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:28:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions", without override shadows
// (-fno-contracts-prove-override, DECISIONS.md E157): the overrider must
// establish the overridden functions' postconditions as well as its own,
// with no premise from their preconditions -- obligations of its own,
// proven in its ordinary shadow -- and its preconditions must be
// tautologies.  The diagnostics still name the overridden function.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fno-contracts-prove-override -fcontract-analysis-semantic=prove -O2" }
// { dg-additional-options "" }

struct B {
  virtual int f (int x) pre (x > 0) post (r: r > 0);	// { dg-message "the predicate of the contract assertion" }
  virtual int g (int x) pre (x > 0) post (r: r > 0);
  virtual int h (int x) pre (x > 0);
};

struct D : B {
  // Proven with an override shadow, from B::f's precondition; not
  // without one.
  int f (int x) override	// { dg-error "cannot be proven for a call through 'virtual int B::f\\(int\\)'" }
  { return x; }
  // Proven without B::g's precondition.
  int g (int x) override	// { dg-bogus "cannot be proven" }
  { return x > 0 ? x : 1; }
  // A tautology, settled by folding.
  int h (int x) override pre (true) { return x; }	// { dg-bogus "cannot be proven" }
};

void use () { D d; }

// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
