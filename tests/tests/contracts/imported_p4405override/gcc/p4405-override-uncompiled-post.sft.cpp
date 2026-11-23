//remark: imported from gcc:p4405-override-uncompiled-post.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:39:[0-9]+: error: 
//match_regex: Test_name.c:41:[0-9]+: error: 
//match_regex: Test_name.c:55:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions", DECISIONS.md E150 and E152: the
// override obligations of the overridden functions' postconditions and of
// destructors -- user-provided or implicitly defined -- in a virtual
// function this translation unit never compiles (an inline overrider whose
// class's vtable is emitted elsewhere, here because the key function is
// defined in another translation unit) are not diagnosed; that translation
// unit compiles the overrider and analyzes it.  Kept and used, the same
// class is diagnosed.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-analysis-semantic=prove -O2" }

int g;

struct B {
  virtual int f (int x) post (r: r > 0);	// { dg-message "the predicate of the contract assertion" }
  virtual ~B () post (g == 1);			// { dg-message "the predicate of the contract assertion" }
};

struct Elsewhere : B {
  virtual void key ();		// defined in another translation unit
  int f (int x) override	// { dg-bogus "cannot be proven" }
  { return x; }
  ~Elsewhere () override	// { dg-bogus "cannot be proven" }
  {}
};

struct Here : B {
  virtual void key ();
  int f (int x) override	// { dg-error "cannot be proven for a call through 'virtual int B::f\\(int\\)'" }
  { return x; }
  ~Here () override		// { dg-error "cannot be proven for a call through 'virtual B::~B\\(\\)'" }
  {}
};

void Here::key () {}

// An implicitly defined destructor, defined for an inline function this
// translation unit does not emit either.
struct ImplicitElsewhere : B {	// { dg-bogus "cannot be proven" }
  virtual void key ();
};

inline void unused (ImplicitElsewhere *p) { p->~ImplicitElsewhere (); }

struct ImplicitHere : B {	// { dg-error "cannot be proven for a call through 'virtual B::~B\\(\\)'" }
  virtual void key ();
};

void ImplicitHere::key () {}
