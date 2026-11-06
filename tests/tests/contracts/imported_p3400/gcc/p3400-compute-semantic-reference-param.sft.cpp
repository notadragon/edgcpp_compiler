//remark: imported from gcc:p3400-compute-semantic-reference-param.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
// P3400: a compute_semantic facet taking `const evaluation_semantic&' is
// applied: the facet is called with an lvalue of evaluation_semantic, as the
// library concept does, rather than a constant of its parameter type.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;

struct by_ref_t {
  using assertion_control_object = by_ref_t;
  constexpr evaluation_semantic
  compute_semantic (const evaluation_semantic& s) const
  { return s == evaluation_semantic::enforce ? evaluation_semantic::observe : s; }
};
constexpr by_ref_t by_ref{};
static_assert (std::contracts::labels::semantic_computation_label<by_ref_t>);

static int violations = 0;
void handle_contract_violation (const std::contracts::contract_violation&)
{ ++violations; }

void f (int x) pre<by_ref> (x > 0) {}

int main () {
  f (-1);                           // enforce -> observe: continues
  if (violations != 1)
    __builtin_abort ();
}
