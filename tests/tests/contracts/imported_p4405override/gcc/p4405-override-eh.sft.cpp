//remark: imported from gcc:p4405-override-eh.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:36:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions": the overridden function's postconditions
// are proven at each return of the overrider -- not where it exits by an
// exception -- for a noexcept overrider, a throwing one, and one returning
// a class by the named return value.  None of these overriders has
// contract assertions of its own: the try/finally that places the
// obligations after the body exists in the override shadows alone
// (p4405-override-realcode.C).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-analysis-semantic=prove -O2" }

struct R { R (); R (const R &); ~R (); int v; };

struct B {
  virtual int f (int x) noexcept pre (x > 0) post (r: r > 0);
  virtual int g (int x) pre (x > 0) post (r: r > 0);
  virtual int h (int x) pre (x > 0) post (r: r > 0);	// { dg-message "the predicate of the contract assertion" }
  virtual R make (int x) pre (x > 0) post (r: r.v > 0);
};

struct D : B {
  int f (int x) noexcept override { return x; }	// { dg-bogus "cannot be proven" }
  int g (int x) override			// { dg-bogus "cannot be proven" }
  {
    if (x > 100)
      throw 1;
    return x;
  }
  int h (int x) override			// { dg-error "cannot be proven for a call through 'virtual int B::h\\(int\\)'" }
  {
    if (x > 100)
      throw 1;
    return x - 1;
  }
  R make (int x) override			// { dg-bogus "cannot be proven" }
  {
    R r;
    r.v = x;
    return r;
  }
};

void use () { D d; }

