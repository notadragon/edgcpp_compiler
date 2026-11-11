//remark:contracts: without --contracts_p4451 "<terminating>" is diagnosed on a lambda and a member function too, pointing at the option (M2-Clang's cases)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--c++26 --contracts
//match_regex:line 10: error: assertion-control specifier <terminating> requires --contracts_p4451
//match_regex:line 12: error: assertion-control specifier <terminating> requires --contracts_p4451
// Mirror: clang/test/Contracts/p4451-diag-no-flag.cpp in the
// llvm_llvm-project fork.
#include <contracts>
auto l = [](int x) pre<terminating>(x > 0) { return x; };
struct S {
  void m(int x) pre<terminating>(x > 0);
};
