//remark:contracts: without --contracts_p4451 (which C++26 enables, so it is turned off here) an assertion-control specifier naming enforced, unqualified or qualified (P4451R2), is diagnosed on a lambda, a member function and a contract_assert too, pointing at the option (M2-Clang's cases)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--c++26 --contracts --no_contracts_p4451
//match_regex:line 12: error: an assertion-control specifier naming enforced requires --contracts_p4451
//match_regex:line 14: error: an assertion-control specifier naming enforced requires --contracts_p4451
//match_regex:line 15: error: an assertion-control specifier naming enforced requires --contracts_p4451
//match_regex:line 17: error: an assertion-control specifier naming enforced requires --contracts_p4451
// Mirror: clang/test/Contracts/p4451-diag-no-flag.cpp in the
// llvm_llvm-project fork.
#include <contracts>
auto l = [](int x) pre<enforced>(x > 0) { return x; };
struct S {
  void m(int x) pre<std::contracts::controls::enforced>(x > 0);
  void n(int x) pre<::std::contracts::controls::enforced>(x > 0);
};
void g(int x) { contract_assert<std::contracts::controls::enforced>(x > 0); }
