//remark:contracts: P3400 the facets of a label are found on a template's instances, a precondition and an in-class member function's postcondition too (review: enforce becomes observe, so a warning)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--contracts_p3400 --contract_evaluation_semantic=enforce
//match_regex:line 10: warning: contract predicate is false in constant expression$
//match_regex:line 12: warning: contract predicate is false in constant expression$
//match_regex:line 16: warning: contract predicate is false in constant expression$
#include <contracts>
namespace sc = std::contracts;
template <class L> constexpr bool t() { contract_assert<L{}>(false); return true; }
static_assert(t<sc::labels::review_t>());
constexpr int f(int x) pre<sc::labels::review>(x > 0) { return x; }
static_assert(f(0) == 0);
struct S {
  constexpr int m(const int x) const
    post<sc::labels::review>(r: r > x) { return x; }
};
static_assert(S{}.m(0) == 0);
