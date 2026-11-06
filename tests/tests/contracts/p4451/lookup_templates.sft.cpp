//remark:contracts: the lookup check of "<terminating>" (P4451) in templates: a function template's parameter, a class template's later member and a block-scope variable in a function template are each diagnosed once, whatever the instances
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--contracts_p4451
//match_regex:line 9: error: unqualified lookup of "terminating" from this assertion-control specifier finds a declaration other than std::contracts::labels::terminating
//match_regex:line 11: error: unqualified lookup of "terminating" from this assertion-control specifier finds a declaration other than std::contracts::labels::terminating
//match_regex:line 18: error: unqualified lookup of "terminating" from this assertion-control specifier finds a declaration other than std::contracts::labels::terminating
#include <contracts>
template <class T> void f(T terminating) pre<terminating>(true);
template <class T> struct S {
  void g() pre<terminating>(true);
  static constexpr int terminating = 0;
};
void use() { f(1); f(2.0); S<int> s; s.g(); S<long> t; t.g(); }
template <class T>
void h(T x) {
  int terminating = 0;
  contract_assert<terminating>(x > 0);
}
void use2() { h(1); h(2L); }
