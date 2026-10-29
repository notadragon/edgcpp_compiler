//remark: imported from gcc:p4405-analysis-elided-temporary-aggregate.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 -O0
//match_regex: Test_name.c:69:[0-9]+: error: 
//match_regex: Test_name.c:72:[0-9]+: error: 
//match_regex: Test_name.c:73:[0-9]+: error: 
//match_regex: Test_name.c:75:[0-9]+: error: 
//match_regex: Test_name.c:81:[0-9]+: error: 
//match_regex: Test_name.c:83:[0-9]+: error: 
//match_regex: Test_name.c:106:[0-9]+: error: 
//match_regex: Test_name.c:107:[0-9]+: error: 
//match_regex: Test_name.c:112:[0-9]+: error: 
//match_regex: Test_name.c:114:[0-9]+: error: 
//match_regex: Test_name.c:115:[0-9]+: error: 
//match_regex: Test_name.c:116:[0-9]+: error: 
//match_regex: Test_name.c:117:[0-9]+: error: 
//match_regex: Test_name.c:118:[0-9]+: error: 
//match_regex: Test_name.c:122:[0-9]+: error: 
//match_regex: Test_name.c:123:[0-9]+: error: 
//match_regex: Test_name.c:129:[0-9]+: error: 
//match_regex: Test_name.c:130:[0-9]+: error: 
//match_regex: Test_name.c:131:[0-9]+: error: 
//match_regex: Test_name.c:132:[0-9]+: error: 
//match_regex: Test_name.c:133:[0-9]+: error: 
//match_regex: Test_name.c:139:[0-9]+: error: 
//match_regex: Test_name.c:141:[0-9]+: error: 
//match_regex: Test_name.c:142:[0-9]+: error: 
//match_regex: Test_name.c:144:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405 ("The Analysis Evaluation"): when an aggregate's initialization
// throws part-way, the members already built are destroyed; that cleanup is
// a destructor call made in the initialization, so it has an analysis
// evaluation whatever the aggregate's storage duration: in a function, for
// a static local, at namespace scope and for a static data member (the
// static initialization function makes the call).  The member's own
// initializer is elided (`DP ()', `make ()'), so it adds nothing.  A's
// implicit destructor's call of `~DP' is analyzed in A's destructor, once.
// Without optimization (decided by folding, at once); code generation, not
// -fsyntax-only, which diagnoses no analysis evaluation.
// Mirror: edgcpp_compiler tests/tests/contracts/p4405/elided_temporary_cgen
// (EDG-150).
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
  ~DP () pre<always_prove> (g > 0);	// { dg-message "predicate of the contract assertion" }
};
DP make ();
int thr ();
struct A { DP p; int i; };	// { dg-error "contract assertion cannot be proven" }

// Namespace scope.
A na{DP (), thr ()};	// { dg-error "contract assertion cannot be proven" }
A nb{make (), thr ()};	// { dg-error "contract assertion cannot be proven" }
struct SA { static A sa; };
A SA::sa{DP (), thr ()};	// { dg-error "contract assertion cannot be proven" }

// In a function.
void
f ()
{
  A b{DP (), thr ()};	// { dg-error "contract assertion cannot be proven" }
  // { dg-message "synthesized method" "" { target *-*-* } .-1 }
  static A c{DP (), thr ()};	// { dg-error "contract assertion cannot be proven" }
  (void) b;
  (void) c;
}

// DECISIONS.md E160 (a): the member's destructor is potentially invoked
// ([dcl.init.aggr]) until the initialization completes, even when nothing
// after it can throw (`A e1{DP (), 1};', a `noexcept' call after it): the
// cleanup GCC builds is analyzed all the same -- for a base, a member
// initialized by parentheses, a default member initializer, a lambda's
// capture and an array's element too.  A later initialization is one the
// initializer or a default member initializer gives; a member left to
// implicit value-initialization (`A e6{DP ()};') is not one, and the last
// member (`L e7{1, DP ()};') has nothing after it.  The variables of static
// storage duration of a type whose destructor has the precondition get the
// address-of error besides.  Mirror: llvm_llvm-project
// clang/test/Contracts/p4405-analysis-elided-temporary-aggregate.cpp.
int nothr () noexcept;
struct DN {
  DN () noexcept;
  ~DN () pre<always_prove> (g > 0);	// { dg-message "predicate of the contract assertion" }
  // { dg-message "precondition here" "" { target *-*-* } .-1 }
};
struct AI { DP p; int i = 5; };	// { dg-error "contract assertion cannot be proven" }
struct L { int i; DP p; };	// { dg-error "contract assertion cannot be proven" }
// D declares its destructor (an aggregate may), defined in no translation
// unit here, so only its base's partial cleanup is analyzed.
struct D : DP { int i; ~D (); };
// An array member's elements are destroyed in a loop.
struct AR { DN a[2]; int i; };	// { dg-error "contract assertion cannot be proven" }

A e1{DP (), 1};			// { dg-error "contract assertion cannot be proven" }
A e2{DP (), nothr ()};		// { dg-error "contract assertion cannot be proven" }
A e3(DP (), 1);			// { dg-error "contract assertion cannot be proven" }
D e4{{}, 1};			// { dg-error "contract assertion cannot be proven" }
AI e5{DP ()};			// { dg-error "contract assertion cannot be proven" }
A e6{DP ()};			// { dg-bogus "cannot be proven" }
L e7{1, DP ()};			// { dg-bogus "cannot be proven" }
struct SE { static A se; };
A SE::se{DP (), 1};		// { dg-error "contract assertion cannot be proven" }
DN e8[2] = {DN (), DN ()};	// { dg-error "contract assertion cannot be proven" }
// { dg-message "of static storage duration" "" { target *-*-* } .-1 }

void
h ()
{
  A b1{DP (), 1};		// { dg-error "contract assertion cannot be proven" }
  static A b2{DP (), nothr ()};	// { dg-error "contract assertion cannot be proven" }
  A b3(DP (), 1);		// { dg-error "contract assertion cannot be proven" }
  D b4{{}, 1};			// { dg-error "contract assertion cannot be proven" }
  static AI b5{DP ()};		// { dg-error "contract assertion cannot be proven" }
  // { dg-message "synthesized method" "" { target *-*-* } .-1 }
  A b6{DP ()};			// { dg-bogus "cannot be proven" }
  L b7{1, DP ()};		// { dg-bogus "cannot be proven" }
  // { dg-message "synthesized method" "" { target *-*-* } .-1 }
  // The closure's destructor, and the capture's partial cleanup.
  auto b8 = [p = DP (), i = 1]	// { dg-error "contract assertion cannot be proven" }
  // { dg-message "synthesized method" "" { target *-*-* } .-1 }
    {};				// { dg-error "contract assertion cannot be proven" }
  static DN b9[2] = {DN (), DN ()};	// { dg-error "contract assertion cannot be proven" }
  // { dg-message "of static storage duration" "" { target *-*-* } .-1 }
  AR b10{{DN (), DN ()}, 1};		// { dg-error "contract assertion cannot be proven" }
  // { dg-message "synthesized method" "" { target *-*-* } .-1 }
  (void) b1;
  (void) b2;
  (void) b3;
  (void) b4;
  (void) b5;
  (void) b6;
  (void) b7;
  (void) b8;
  (void) b10;
}
