//remark: imported from gcc:p3595-dynamic-allowed.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-dynamic-allowed.json
//options: --c++26 --contracts --contracts_p3400 --contract_configuration_file=p3595-dynamic-allowed.json
//use_system_includes: true
//linker_options: -lcontracts
// P3595: a returned semantic outside allowed_semantics is clamped (fallback_order).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-dynamic-allowed.json" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
#include <contracts>
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

// allowed_semantics = { observe } only.
struct only_observe_t {
  using assertion_control_object = only_observe_t;
  static constexpr evaluation_semantic_set allowed_semantics =
    {evaluation_semantic::observe};
};
constexpr only_observe_t only_observe{};

static int violations = 0;
void handle_contract_violation(const std::contracts::contract_violation&) {
  ++violations;
}

// Selector returns enforce, which is NOT allowed -> clamp to observe.
evaluation_semantic p3595_sel_allowed() { return evaluation_semantic::enforce; }

void f(int x) pre<only_observe>(x > 0) { }

int main() {
  f(-1);                       // enforce -> clamped to observe -> handler+continue
  if (violations != 1) __builtin_abort();
}
