//remark: imported from gcc:contract-enclosing-lambda-capture-constify.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 41: (?:catastrophic )?error
//match_regex: ", line 42: (?:catastrophic )?error
//match_regex: ", line 43: (?:catastrophic )?error
//match_regex: ", line 44: (?:catastrophic )?error
//match_regex: ", line 45: (?:catastrophic )?error
//match_regex: ", line 52: (?:catastrophic )?error
//match_regex: ", line 53: (?:catastrophic )?error
//match_regex: ", line 54: (?:catastrophic )?error
//match_regex: ", line 65: (?:catastrophic )?error
//match_regex: ", line 66: (?:catastrophic )?error
//match_regex: ", line 67: (?:catastrophic )?error
//match_regex: ", line 68: (?:catastrophic )?error
//match_regex: ", line 69: (?:catastrophic )?error
//match_regex: ", line 77: (?:catastrophic )?error
//match_regex: ", line 78: (?:catastrophic )?error
// A contract assertion in the body of a lambda: an entity that lambda
// captured is declared outside the contract assertion, so it is const in the
// predicate ([expr.prim.id.unqual]) -- also when a lambda in the predicate
// captures it by reference in turn, whether the enclosing lambda captured
// it by reference or by copy (a mutable one may modify its own copy outside
// the contract assertion), implicitly, explicitly or with an init-capture,
// and whatever the entity is (a scalar, an array, a VLA).  These used to
// compile: the predicate's lambda took the enclosing lambda's capture for
// one made inside the predicate.  A by-copy capture made by a lambda that is
// itself in the predicate still cuts the chain.
//
// Mirror: clang/test/Contracts/contract-enclosing-lambda-capture-constify.cpp
// in the llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -Wno-vla" }

int take (int *);

void
by_ref (int x)
{
  auto l1 = [&] { (void) x; contract_assert ([&] { return ++x; } ()); }; // { dg-error "read-only" }
  auto l2 = [&] { contract_assert ([&] { return ++x; } ()); (void) x; }; // { dg-error "read-only" }
  auto l3 = [&x] { contract_assert ([&] { return ++x; } ()); };	// { dg-error "read-only" }
  auto l4 = [&r = x] { contract_assert ([&r] { return ++r; } ()); }; // { dg-error "read-only" }
  auto l5 = [&] { (void) x; contract_assert ([&] { return [&] { return ++x; } (); } ()); }; // { dg-error "read-only" }
  l1 (); l2 (); l3 (); l4 (); l5 ();
}

void
by_copy (int x)
{
  auto l1 = [=] () mutable { ++x; contract_assert ([&] { return ++x; } ()); }; // { dg-error "read-only" }
  auto l2 = [y = x] () mutable { ++y; contract_assert ([&] { return ++y; } ()); }; // { dg-error "read-only" }
  auto l3 = [x] () mutable { ++x; contract_assert ([&x] { return ++x; } ()); }; // { dg-error "read-only" }
  // The predicate's own copy is its own.
  auto l4 = [=] () mutable { ++x; contract_assert ([=] () mutable { return [&] { return ++x; } (); } ()); };
  l1 (); l2 (); l3 (); l4 ();
}

void
arrays (int n)
{
  int a[3] = {};
  int v[n];
  auto l1 = [&] { (void) a; contract_assert ([&] { return ++a[0]; } ()); }; // { dg-error "read-only" }
  auto l2 = [&] { (void) v; contract_assert ([&] { return ++v[0]; } ()); }; // { dg-error "read-only" }
  auto l3 = [&] { (void) v; contract_assert ([&] { return take (v); } ()); }; // { dg-error "invalid conversion" }
  auto l4 = [&v] { contract_assert ([&v] { return take (v); } ()); }; // { dg-error "invalid conversion" }
  auto l5 = [=] () mutable { ++a[0]; contract_assert ([&] { return take (a); } ()); }; // { dg-error "invalid conversion" }
  l1 (); l2 (); l3 (); l4 (); l5 ();
}

template <class T>
void
tby_ref (T x)
{
  auto l1 = [&] { (void) x; contract_assert ([&] { return ++x; } ()); }; // { dg-error "read-only" }
  auto l2 = [=] () mutable { ++x; contract_assert ([&] { return ++x; } ()); }; // { dg-error "read-only" }
  l1 (); l2 ();
}
template void tby_ref<int> (int);
