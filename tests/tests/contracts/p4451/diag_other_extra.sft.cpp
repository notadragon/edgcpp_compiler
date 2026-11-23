//remark:contracts: under --contracts_p4451 alone an assertion-control specifier that names something other than std::contracts::controls::enforced (here a P3400-style assertion-control object) is diagnosed on a lambda and a member function too, once each, and std::contracts::controls::enforced is not an assertion-control object (M2-Clang's cases)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--c++26 --contracts_p4451
//match_regex:line 11: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 13: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
// Mirror: clang/test/Contracts/p4451-diag-other.cpp in the
// llvm_llvm-project fork.
#include <contracts>
struct other_t { using assertion_control_object = other_t; } other;
auto l = [](int x) pre<other>(x > 0) { return x; };
struct S {
  void m(int x) pre<other>(x > 0);
  void n(int x) pre<std::contracts::controls::enforced>(x > 0);  // OK
};
template <class T>
concept control = requires { typename T::assertion_control_object; };
static_assert(!control<decltype(std::contracts::controls::enforced)>);
