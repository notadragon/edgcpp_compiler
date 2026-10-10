//remark:contracts: P3400 labels on preconditions, postconditions and contract_assert are put out for g++, which applies their facets at run time
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contracts_p3400 --contract_evaluation_semantic=enforce
//match_regex:^violations 4$
#include <contracts>
#include <cstdio>

static int n = 0;
void handle_contract_violation(const std::contracts::contract_violation &) {
  ++n;
}

namespace mylabels {
  struct observe_t {
    using assertion_control_object = observe_t;
    constexpr std::contracts::evaluation_semantic
    compute_semantic(std::contracts::evaluation_semantic) const {
      return std::contracts::evaluation_semantic::observe;
    }
  };
  inline constexpr observe_t always_observe{};
}
// Names found only within assertion-control specifiers and contract_control.
using contract_control namespace mylabels;

int f(int x) pre<review>(x > 0) { return x; }
int g(int x) post<always_observe>(r: r > 0) { return x; }
struct S {
  int m;
  int h() const pre<(always_observe)>(m > 0) { return m; }
};
template <class T> T t(T x) pre<review>(x > 0) { return x; }

static_assert(contract_control(review).compute_semantic(
                  std::contracts::evaluation_semantic::enforce) ==
              std::contracts::evaluation_semantic::observe);
constexpr auto my_label = contract_control(always_observe | review);

int main() {
  f(0);                                     // observed
  g(0);                                     // observed
  S{0}.h();                                 // observed
  t(0);                                     // observed
  contract_assert<review>(n == 4);          // holds
  std::printf("violations %d\n", n);
  return 0;
}
