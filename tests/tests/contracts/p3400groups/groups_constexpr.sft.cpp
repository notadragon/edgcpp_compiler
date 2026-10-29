//remark:contracts: P3400 a label's group names match the configuration's group entries in constant evaluation: the group itself or a dot-separated prefix of it, also for a combined label
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_sep:!
//options:--contracts_p3400 --contract_evaluation_semantic=enforce --contract_group_evaluation_semantic=safety:ignore --contract_group_evaluation_semantic=a:observe
//match_regex:line 13: warning: contract predicate is false in constant expression$
//match_regex:line 14: error: contract predicate is false in constant expression$
//match_regex:^1 error detected
// f() and k() (safety: ignore) report nothing.
#include <contracts>
using namespace std::contracts::labels;
constexpr bool f() { contract_assert<"safety"group>(false); return true; }
constexpr bool g() { contract_assert<"a.b"group>(false); return true; }
constexpr bool h() { contract_assert<"ab"group>(false); return true; }
constexpr bool k() { contract_assert<("x"group | "safety"group)>(false); return true; }
static_assert(f());
static_assert(g());
static_assert(h());
static_assert(k());
