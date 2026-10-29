//remark: imported from gcc:p4405-analysis-ctor-proof.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -O2
//match_regex: Test_name.c:89:[0-9]+: error: 
//match_regex: Test_name.c:90:[0-9]+: error: 
//match_regex: Test_name.c:91:[0-9]+: error: 
//match_regex: Test_name.c:92:[0-9]+: error: 
//match_regex: Test_name.c:93:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405 ("The Analysis Evaluation"): a precondition's analysis evaluation
// at a call of a constructor, or of a function returning a class, is
// deferred to the proof shadow like any other, so a guarded call is proven
// when optimizing (not only by folding).  The call itself stays a call: a
// constructor call carries the evaluation in its object argument, and a
// call initializing a temporary (a prvalue of class type) in the
// temporary's initializer, ahead of the call; both are evaluated before
// the call (p4405-analysis-ctor-proof-noarg.C has calls with no other
// argument).  Unguarded calls still cannot be proven,
// and a constant argument is folded.  An unproven evaluation is a premise for
// those after it, so each unguarded call has its own variable.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

struct always_prove_t {
  using assertion_control_object = always_prove_t;
  static constexpr evaluation_semantic_set
    allowed_semantics{evaluation_semantic::prove};
};
constexpr always_prove_t always_prove{};

struct T {
  int v;
  T (int x) pre<always_prove> (x > 0) : v (x) {}	// { dg-message "the predicate of the contract assertion" }
  T (int x, const int &y) pre<always_prove> (x > y) : v (x - y) {}	// { dg-message "the predicate of the contract assertion" }
  ~T () {}
  T scaled (int k) const pre<always_prove> (k > 0) { T r = *this; r.v *= k; return r; }	// { dg-message "the predicate of the contract assertion" }
};

struct V : virtual T {
  V (int x) pre<always_prove> (x > 1) : T (x) {}	// { dg-message "the predicate of the contract assertion" }
};

struct R { int v; };
R mk (int x) pre<always_prove> (x > 0);	// { dg-message "the predicate of the contract assertion" }

int
guarded (int y, int z)
{
  int r = 0;
  if (y > 0)
    {
      T t (y);					// { dg-bogus "cannot be proven" }
      r += t.v;
    }
  if (y > 0)
    r += T (y).v;				// { dg-bogus "cannot be proven" }
  if (y > 0)
    r += (new T (y))->v;			// { dg-bogus "cannot be proven" }
  if (y > z)
    {
      T t (y, z);				// { dg-bogus "cannot be proven" }
      r += t.v;
    }
  if (y > 0)
    r += mk (y).v;				// { dg-bogus "cannot be proven" }
  if (y > 0)
    r += T (1).scaled (y).v;			// { dg-bogus "cannot be proven" }
  if (y > 1)
    {
      V w (y);					// { dg-bogus "cannot be proven" }
      r += w.v;
    }
  T c (1);					// { dg-bogus "cannot be proven|always false" }
  return r + c.v;
}

int
unguarded (int a, int b, int c, int d, int e, int f)
{
  T t (a);					// { dg-error "contract assertion cannot be proven" }
  T u (b, c);					// { dg-error "contract assertion cannot be proven" }
  R r = mk (d);					// { dg-error "contract assertion cannot be proven" }
  V w (e);					// { dg-error "contract assertion cannot be proven" }
  return t.v + u.v + r.v + w.v + T (1).scaled (f).v;	// { dg-error "contract assertion cannot be proven" }
}
