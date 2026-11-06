//remark: imported from gcc:p4405-analysis-elided-temporary.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -O0
//match_regex: Test_name.c:58:[0-9]+: error: 
//match_regex: Test_name.c:61:[0-9]+: error: 
//match_regex: Test_name.c:64:[0-9]+: error: 
//match_regex: Test_name.c:69:[0-9]+: error: 
//match_regex: Test_name.c:71:[0-9]+: error: 
//match_regex: Test_name.c:98:[0-9]+: error: 
//match_regex: Test_name.c:99:[0-9]+: error: 
//match_regex: Test_name.c:100:[0-9]+: error: 
//match_regex: Test_name.c:113:[0-9]+: error: 
//match_regex: Test_name.c:128:[0-9]+: error: 
//match_regex: Test_name.c:134:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// The namespace-scope initializers of `z' and `t' are reported on the
// declaration's line: the C++-generating back end puts each initializer
// on its declarator's line, which is the line g++ sees.
// D4405 ("The Analysis Evaluation"): the destructor call of a temporary
// that an initialization elides is never made, so it has no analysis
// evaluation (GCC-944): only a temporary that is materialized and destroyed
// has one.  At namespace scope and in a function, without optimization
// (decided by folding, at once).  The variables of static storage duration
// still get their address-of error (D4405R0, "Taking the Address of a
// Function"); an automatic variable's own destruction is analyzed at the end
// of its scope, so that case is elided into an object nobody destroys
// (`new') or into a return value.  Code generation, not -fsyntax-only,
// which diagnoses no analysis evaluation.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O0" }
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

int g;

struct DP {
  DP ();
  ~DP () pre<always_prove> (g > 0);	// { dg-message "precondition here" }
};
DP make ();
void take (DP);

// Namespace scope: the static initialization function makes the calls.
DP x =	// { dg-error "for a call through the address of 'DP::~DP\\(\\)'" }
  DP ();	// { dg-bogus "cannot be proven" }
// { dg-message "'x', of static storage duration" "" { target *-*-* } .-2 }
DP y =	// { dg-error "for a call through the address of 'DP::~DP\\(\\)'" }
  make ();	// { dg-bogus "cannot be proven" }
// { dg-message "'y', of static storage duration" "" { target *-*-* } .-2 }
DP w =	// { dg-error "for a call through the address of 'DP::~DP\\(\\)'" }
  g ? DP () : make ();	// { dg-bogus "cannot be proven" }
// { dg-message "'w', of static storage duration" "" { target *-*-* } .-2 }
DP *np = new DP (DP ());	// { dg-bogus "cannot be proven" }
DP *mp = new DP (make ());	// { dg-bogus "cannot be proven" }
int z =
  (DP (), 1);	// { dg-error "contract assertion cannot be proven" }
int t =
  (take (DP ()), 1);	// { dg-error "contract assertion cannot be proven" }

// In a function.
DP
ret ()
{
  return DP ();	// { dg-bogus "cannot be proven" }
}

DP
ret_make ()
{
  return make ();	// { dg-bogus "cannot be proven" }
}

DP
ret_cond ()
{
  return g ? DP () : make ();	// { dg-bogus "cannot be proven" }
}

void
f ()
{
  DP *p = new DP (DP ());	// { dg-bogus "cannot be proven" }
  DP *q = new DP (make ());	// { dg-bogus "cannot be proven" }
  int a = (DP (), 1);	// { dg-error "contract assertion cannot be proven" }
  take (DP ());	// { dg-error "contract assertion cannot be proven" }
  take (make ());	// { dg-error "contract assertion cannot be proven" }
}

// An inline function nobody calls: still analyzed, as any other.
inline DP
unused_ret ()
{
  return DP ();	// { dg-bogus "cannot be proven" }
}

inline int
unused_kept ()
{
  return (DP (), 1);	// { dg-error "contract assertion cannot be proven" }
}

// A template, in its instantiation.
template <typename T>
T
tret ()
{
  return T ();	// { dg-bogus "cannot be proven" }
}

template <typename T>
int
tkept ()
{
  return (T (), 1);	// { dg-error "contract assertion cannot be proven" }
}

void
h ()
{
  delete new DP (tret<DP> ());	// { dg-error "contract assertion cannot be proven" }
  tkept<DP> ();
}
