//remark:contracts: P3400 the facets of a label choose the semantic of a contract assertion in constant evaluation (allowed_semantics, compute_semantic) and its message (compute_message), as GCC does; a private facet is absent
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--contracts_p3400 --contracts_p3099 --contract_evaluation_semantic=enforce
//match_regex:line 53: warning: contract predicate is false in constant expression$
//match_regex:line 54: error: contract predicate is false in constant expression \(computed\)$
//match_regex:line 56: error: compute_semantic result is not in the allowed evaluation semantics
//match_regex:line 56: error: contract predicate is false in constant expression$
//match_regex:line 57: error: assertion-control label allows no evaluation semantics
//match_regex:line 58: error: the compute_semantic facet of assertion-control object "NonConst" must be usable in a constant expression
//match_regex:line 59: error: contract predicate is false in constant expression$
//match_regex:^6 errors detected
// c() and h() (only ignore allowed) report nothing.
#include <contracts>
namespace sc = std::contracts;
using sc::evaluation_semantic;

struct OnlyIgnore {
  using assertion_control_object = OnlyIgnore;
  const sc::evaluation_semantic_set allowed_semantics{evaluation_semantic::ignore};
};
struct BadCompute {
  using assertion_control_object = BadCompute;
  const sc::evaluation_semantic_set allowed_semantics{evaluation_semantic::enforce};
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return evaluation_semantic::observe; }
};
struct NoneAllowed {
  using assertion_control_object = NoneAllowed;
  const sc::evaluation_semantic_set allowed_semantics{};
};
struct NonConst {
  using assertion_control_object = NonConst;
  evaluation_semantic compute_semantic(evaluation_semantic s) const { return s; }
};
struct Msg {
  using assertion_control_object = Msg;
  constexpr const char *compute_message(const char *) const { return "computed"; }
};
class Private {
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const
  { return evaluation_semantic::ignore; }
public:
  using assertion_control_object = Private;
};
constexpr OnlyIgnore only_ignore{};
constexpr BadCompute bad_compute{};
constexpr NoneAllowed none_allowed{};
constexpr NonConst non_const{};
constexpr Msg msg{};
constexpr Private priv{};

constexpr bool a() { contract_assert<sc::labels::review>(false); return true; }
constexpr bool b() { contract_assert<msg>(false, "original"); return true; }
constexpr bool c() { contract_assert<only_ignore>(false); return true; }
constexpr bool d() { contract_assert<bad_compute>(false); return true; }
constexpr bool e() { contract_assert<none_allowed>(true); return true; }
constexpr bool f() { contract_assert<non_const>(true); return true; }
constexpr bool g() { contract_assert<priv>(false); return true; }
constexpr bool h() { contract_assert<(sc::labels::review | only_ignore)>(false); return true; }
static_assert(a());
static_assert(b());
static_assert(c());
static_assert(d());
static_assert(e());
static_assert(f());
static_assert(g());
static_assert(h());
