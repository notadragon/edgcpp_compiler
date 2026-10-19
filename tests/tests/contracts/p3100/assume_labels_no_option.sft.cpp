//remark:contracts: P3100 with P3400 labels without --contracts_allow_assume: a configured assume is ignore, and a label may neither allow nor compute assume, as GCC does
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--contracts_p3400 --contract_evaluation_semantic=assume
//match_regex:line 44: error: no valid evaluation semantic for contract assertion
//match_regex:line 45: warning: contract predicate is false in constant expression
//match_regex:line 47: error: compute_semantic result is not in the allowed evaluation semantics
//match_regex:line 48: warning: contract predicate is false in constant expression
#include <contracts>
namespace sc = std::contracts;
using sc::evaluation_semantic;

// Only assume: assume itself with the option; without it, nothing.
struct OnlyAssume {
  using assertion_control_object = OnlyAssume;
  const sc::evaluation_semantic_set allowed_semantics{
    evaluation_semantic::assume};
};
// Only observe: neither assume nor ignore, so up to observe.
struct OnlyObserve {
  using assertion_control_object = OnlyObserve;
  const sc::evaluation_semantic_set allowed_semantics{
    evaluation_semantic::observe};
};
// ignore and enforce: ignore, the nearest level up from assume.
struct IgnoreEnforce {
  using assertion_control_object = IgnoreEnforce;
  const sc::evaluation_semantic_set allowed_semantics{
    evaluation_semantic::ignore, evaluation_semantic::enforce};
};
// Computes assume from anything (without the option, not allowed).
struct ComputeAssume {
  using assertion_control_object = ComputeAssume;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return evaluation_semantic::assume; }
};
// Computes enforce from assume, observe from the rest.
struct EnforceFromAssume {
  using assertion_control_object = EnforceFromAssume;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic s) const
  { return s == evaluation_semantic::assume ? evaluation_semantic::enforce
                                            : evaluation_semantic::observe; }
};
constexpr int a(int x) pre<OnlyAssume{}>(x > 0) { return x; }
constexpr int b(int x) pre<OnlyObserve{}>(x > 0) { return x; }
constexpr int c(int x) pre<IgnoreEnforce{}>(x > 0) { return x; }
constexpr int d(int x) pre<ComputeAssume{}>(x > 0) { return x; }
constexpr int e(int x) pre<EnforceFromAssume{}>(x > 0) { return x; }
// a no semantic (an error), b observe, c ignore, d an error (assume is not
// allowed), e observe (from ignore).
constexpr int va = a(0);
constexpr int vb = b(0);
constexpr int vc = c(0);
constexpr int vd = d(0);
constexpr int ve = e(0);
