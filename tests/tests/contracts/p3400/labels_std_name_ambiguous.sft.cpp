//remark:contracts: P3400 a user label named like one of <contracts>'s std::contracts::labels is ambiguous unless qualified
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options_all:--contracts_p3400 --contract_evaluation_semantic=enforce
//match_regex:line 23: error: "always_observe" is ambiguous
//match_regex:line 28: error: "terminating" is ambiguous
// A label is looked up in std::contracts::labels too, and <contracts> now
// declares always_observe, terminating and the other semantic restriction
// labels there (P3400R4), so a label of the same name at namespace scope, or
// in a namespace named by "using contract_control namespace", is ambiguous;
// a qualified name is not.  Found by M30-GCC, whose tests declared their own
// terminating and always_observe; GCC and Clang agree (M34-EDG).
#include <contracts>
using std::contracts::evaluation_semantic;
struct fixed_t {
  using assertion_control_object = fixed_t;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic) const {
    return evaluation_semantic::observe;
  }
};
constexpr fixed_t always_observe{};

int f(int x) pre<always_observe>(x > 0) { return x; }
int g(int x) pre<::always_observe>(x > 0) { return x; }
int h(int x) pre<std::contracts::labels::always_observe>(x > 0) { return x; }
namespace ns { constexpr fixed_t terminating{}; }
using contract_control namespace ns;
int k(int x) pre<terminating>(x > 0) { return x; }
int m(int x) pre<ns::terminating>(x > 0) { return x; }
int main() {}
