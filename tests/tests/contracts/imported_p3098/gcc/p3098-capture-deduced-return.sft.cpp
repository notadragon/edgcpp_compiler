//remark: imported from gcc:p3098-capture-deduced-return.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// EDG: adapted -- X::g, declared in its class and defined outside it, is left
// out: a deduced return type, a result name and a fold over a pack in a
// postcondition of a non-defining declaration fail to instantiate (EDG-108,
// watch test openbugs/edg-108-deduced-result-name-fold-redecl).
// A postcondition capture of a function template with a deduced return type
// and a result name.  The predicate is substituted as if in a template until
// the return type is deduced; the captures were too, so an initializer
// naming a member of a dependent object (`t.m') stayed a template tree: a
// segfault in the gimplifier for a pack capture, and for a scalar one a
// capture of dependent decltype type ("incomplete type", then an ICE behind
// the error) (GCC-477).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

int violations;

void
handle_contract_violation (const std::contracts::contract_violation &)
{
  ++violations;
}

struct S { int m; };

template <class T>
auto a (T t) post [c = t.m] (r: c > 0 && r == c) { return t.m; }
template <class T>
auto b (T t) post [c = t.m] (r: true) { return 0; }
template <class... T>
auto p (T... t) post [...c = t.m] (r: ((c > 0) && ...) && r == 0) { return 0; }
template <class... T>
auto q (T... t) post [...c = t.m + 1] (r: ((c > 1) && ...)) { return 0; }


int
main ()
{
  S one{1}, zero{0};
  a (one); b (one); p (one, one); q (one);
  if (violations != 0)
    __builtin_abort ();
  a (zero); p (one, zero); q (zero, one);
  if (violations != 3)
    __builtin_abort ();
}
