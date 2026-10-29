//remark: imported from gcc:p4405-override-dtor-nooverride.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --no_contracts_prove_override --contract_analysis_semantic=prove --contract_evaluation_semantic=observe -O2
//match_regex: Test_name.c:37:[0-9]+: error: 
//match_regex: Test_name.c:40:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions", DECISIONS.md E152 and E157: without
// override shadows (-fno-contracts-prove-override), a virtual destructor --
// an implicitly defined one too -- must establish the overridden
// destructors' postconditions as obligations of its own, proven in its
// ordinary shadow.  In a class with virtual bases only the complete-object
// destructor holds them.  The diagnostics still name the overridden
// destructor.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fno-contracts-prove-override -fcontract-analysis-semantic=prove -fcontract-evaluation-semantic=observe -O2" }
// { dg-additional-options "" }

int g;

struct B {
  virtual ~B () post (g == 1) { g = 1; }
};

struct D : B {			// { dg-bogus "cannot be proven" }
};

int g2;

struct B2 {
  virtual ~B2 () post (g2 == 1);	// { dg-message "the predicate of the contract assertion" }
};

struct E : B2 {
  ~E () override {}		// { dg-error "cannot be proven for a call through 'virtual B2::~B2\\(\\)'" }
};

struct F : B2 {			// { dg-error "cannot be proven for a call through 'virtual B2::~B2\\(\\)'" }
};

struct V : virtual B {		// { dg-bogus "cannot be proven" }
};

void use () { D d; E e; F f; V v; }

// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
// (EDG: a dg-final scan of the original, dropped)
