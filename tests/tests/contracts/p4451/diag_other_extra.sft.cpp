//remark:contracts: under --contracts_p4451 alone any other assertion-control specifier is diagnosed on a lambda and a member function too, and std::contracts::labels::terminating is not an assertion-control object (M2-Clang's cases)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--c++26 --contracts_p4451
//match_regex:line 11: error: only <terminating> is allowed as an assertion-control specifier without --contracts_p3400
//match_regex:line 13: error: only <terminating> is allowed as an assertion-control specifier without --contracts_p3400
// Mirror: clang/test/Contracts/p4451-diag-other.cpp in the
// llvm_llvm-project fork.
#include <contracts>
struct other_t { using assertion_control_object = other_t; } other;
auto l = [](int x) pre<other>(x > 0) { return x; };
struct S {
  void m(int x) pre<other>(x > 0);
  void n(int x) pre<terminating>(x > 0);  // OK
};
template <class T>
concept control = requires { typename T::assertion_control_object; };
static_assert(!control<decltype(std::contracts::labels::terminating)>);
