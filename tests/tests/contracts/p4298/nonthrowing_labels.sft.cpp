//remark:contracts: P4298 with P3400 labels: allowed_semantics and compute_semantic take the noexcept semantics, and the best fit prefers the variant of the configured semantic's kind, as GCC does
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_sep:!
//options:--contracts_p4298 --contracts_p3400 --contract_evaluation_semantic=observe -DP4298=1!--contracts_p3400 --contract_evaluation_semantic=observe -DP4298=0
//match_regex:line 38: (warning: contract predicate is false in constant expression|error: no valid evaluation semantic for contract assertion)
//match_regex:line 39: (warning|error): contract predicate is false in constant expression
//match_regex:line 40: (error|warning): contract predicate is false in constant expression
//match_regex:line 41: (warning|error): contract predicate is false in constant expression
#include <contracts>
namespace sc = std::contracts;
using sc::evaluation_semantic;

// Only noexcept_observe: the best fit for observe (without P4298, nothing).
struct OnlyNoexceptObserve {
  using assertion_control_object = OnlyNoexceptObserve;
  const sc::evaluation_semantic_set allowed_semantics{
    evaluation_semantic::noexcept_observe};
};
// enforce and noexcept_observe: observe's level first, so noexcept_observe.
struct ObserveLevel {
  using assertion_control_object = ObserveLevel;
  const sc::evaluation_semantic_set allowed_semantics{
    evaluation_semantic::noexcept_observe, evaluation_semantic::enforce};
};
// Computes noexcept_enforce (without P4298 not allowed: an error anyway).
struct ComputeNoexceptEnforce {
  using assertion_control_object = ComputeNoexceptEnforce;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return evaluation_semantic::noexcept_enforce; }
};
// Computes noexcept_observe from anything.
struct ComputeNoexceptObserve {
  using assertion_control_object = ComputeNoexceptObserve;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return evaluation_semantic::noexcept_observe; }
};
constexpr int a(int x) pre<OnlyNoexceptObserve{}>(x > 0) { return x; }
constexpr int b(int x) pre<ObserveLevel{}>(x > 0) { return x; }
constexpr int c(int x) pre<ComputeNoexceptEnforce{}>(x > 0) { return x; }
constexpr int d(int x) pre<ComputeNoexceptObserve{}>(x > 0) { return x; }
constexpr int va = a(0);
constexpr int vb = b(0);
constexpr int vc = c(0);
constexpr int vd = d(0);
