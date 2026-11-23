//remark: imported from gcc:p4405-override-after-error.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_analysis_semantic=prove -O2
//match_regex: ", line 21: (?:catastrophic )?error
// EDG: adapted -- the front end reports the undeclared name itself, so the
// compilation stops before g++ sees the test, and g++'s override diagnostics
// of the original do not appear.  The C-generating front end's own rule is
// tested by p4405override/override_after_error_cgen.
// D4405R0, "Virtual Functions", after an error elsewhere in the translation
// unit (GCC-947).  The middle end then does not run: no proof shadow and no
// override shadow is made, and every obligation left to one is diagnosed as
// unproven, as at the end of any compilation that stopped at an error.  An
// override pair's obligations are diagnosed in the same way -- they used to
// be dropped -- and so are those folding decided at once; DECISIONS.md E150
// still holds: an inline overrider whose vtable another translation unit
// emits is not diagnosed here.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-analysis-semantic=prove -O2" }

int bad = undeclared;		// { dg-error "not declared" }

struct B {
  virtual int f (int x)
    pre (x > 0)
    post (r: r > 0);		// { dg-message "the predicate of the contract assertion" }
};

// Its precondition and B::f's postcondition, proven in its override shadow
// without the error, unproven.
struct D : B {
  int f (int x) override	// { dg-error "cannot be proven for a call through 'virtual int B::f\\(int\\)'" }
    pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
  {
    return x;
  }
};

// Its precondition decided at once by folding; B::f's postcondition, proven
// in its override shadow without the error, unproven.
struct F : B {
  int f (int x) override	// { dg-error "always false for a call through 'virtual int B::f\\(int\\)'" }
  // { dg-error "cannot be proven for a call through 'virtual int B::f\\(int\\)'" "" { target *-*-* } .-1 }
    pre (false)			// { dg-message "the predicate of the contract assertion" }
  {
    return 1;
  }
};

// Its vtable is emitted elsewhere (the key function is defined in another
// translation unit): not diagnosed here.
struct Elsewhere : B {
  virtual void key ();
  int f (int x) override	// { dg-bogus "for a call through" }
    pre (false)
  {
    return 1;
  }
};

D d;
F fo;
