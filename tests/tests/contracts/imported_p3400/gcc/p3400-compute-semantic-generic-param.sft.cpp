//remark: imported from gcc:p3400-compute-semantic-generic-param.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
// P3400: a compute_semantic facet taking `auto' is applied; the argument is
// an evaluation_semantic lvalue, so the parameter deduces to that type.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;

struct generic_t {
  using assertion_control_object = generic_t;
  constexpr evaluation_semantic compute_semantic (auto s) const
  { return s == evaluation_semantic::enforce ? evaluation_semantic::observe : s; }
};
constexpr generic_t generic{};
static_assert (std::contracts::labels::semantic_computation_label<generic_t>);

static int violations = 0;
void handle_contract_violation (const std::contracts::contract_violation&)
{ ++violations; }

void f (int x) pre<generic> (x > 0) {}

int main () {
  f (-1);                           // enforce -> observe: continues
  if (violations != 1)
    __builtin_abort ();
}
