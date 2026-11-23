//remark: imported from gcc:p4405-override-dtor-constexpr.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_analysis_semantic=prove --contract_evaluation_semantic=observe -O2
//match_regex: Test_name.c:22:[0-9]+: error: 
//match_regex: Test_name.c:31:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Virtual Functions", DECISIONS.md E152: a constexpr virtual
// destructor, user-provided or implicitly defined, gets its override
// shadows like any other, and constant evaluation of it is unaffected: the
// override regions, and the try/finally that holds the overridden
// destructor's postcondition's obligation, are never executed there.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-analysis-semantic=prove -fcontract-evaluation-semantic=observe -O2" }

int g;

struct CB {
  constexpr virtual ~CB ()
    post (__builtin_is_constant_evaluated () || g == 1)	// { dg-error "contract assertion cannot be proven" }
  {}	// { dg-message "the predicate of the contract assertion" "" { target *-*-* } .-1 }
};

struct CD : CB {
  constexpr ~CD () override	// { dg-bogus "cannot be proven" }
  { if !consteval { g = 1; } }
};

struct CE : CB {		// { dg-error "cannot be proven for a call through '.*CB::~CB\\(\\)'" }
  int i = 0;
};

constexpr bool
destroy ()
{
  CB *p = new CD;
  delete p;
  p = new CE;
  delete p;
  { CD d; CE e; }
  return true;
}

static_assert (destroy ());

void use () { CD d; CE e; }
