//remark: imported from gcc:p3400-label-lambda-specifier.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// EDG: adapted -- the front end does not have GCC's extension spellings
// __pre and __post, so l5 is written with pre (the case is otherwise l2's).
// P3400: a labelled function-contract-specifier can follow a lambda's
// lambda-specifier-seq.  `pre<label>' after `mutable', `static' or
// `constexpr' used to be taken for a template-id naming a type, and the
// lambda was rejected ("expected '{'") (GCC-918).  Each label is applied:
// always_observe checks under the configured ignore.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

namespace L = std::contracts::labels;
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

template<evaluation_semantic S>
struct only {
  using assertion_control_object = only;
  static constexpr evaluation_semantic_set allowed_semantics{S};
};

static int violations;

void handle_contract_violation (const std::contracts::contract_violation &)
{
  ++violations;
}

auto l1 = [] (int x) static pre<L::always_observe> (x > 0) { return x; };
auto l2 = [] (int x) mutable pre<L::always_observe> (x > 0) { return x; };
auto l3 = [] (int x) constexpr post<L::always_observe> (r: r > 0) { return x; };
auto l4 = [] (int x) static constexpr pre <only<evaluation_semantic::observe>{}> (x > 0)
  post<L::always_observe> (r: r > 0) { return x; };
auto l5 = [] (int x) mutable pre<L::always_observe> (x > 0) { return x; };
auto l6 = [] (int x) static -> int pre<L::always_observe> (x > 0) { return x; };
auto l7 = [] (int x) static pre (x > 0) { return x; };

int main ()
{
  l1 (-1);
  l2 (-1);
  l3 (-1);
  l4 (-1);
  l5 (-1);
  l6 (-1);
  l7 (-1);
  if (violations != 7)
    __builtin_abort ();
}
