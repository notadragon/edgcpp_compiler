//remark:contracts: the check of a P4451 assertion-control specifier in templates (P4451R2): a function template's parameter, a class template's later member and a block-scope variable in a function template each name another declaration, and a dependent id-expression (T::enforced, T::N::enforced, a non-type template parameter named enforced) is an error at the definition, never instantiated; each is diagnosed once, whatever the instances
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--contracts_p4451
//match_regex:line 18: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 20: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 27: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 31: error: assertion-control-specifier must name std::contracts::controls::enforced, not a dependent name
//match_regex:line 33: error: assertion-control-specifier must name std::contracts::controls::enforced, not a dependent name
//match_regex:line 34: error: assertion-control-specifier must name std::contracts::controls::enforced, not a dependent name
//match_regex:line 37: error: assertion-control-specifier must name std::contracts::controls::enforced, not a dependent name
//match_regex:line 39: error: assertion-control-specifier must name std::contracts::controls::enforced, not a dependent name
// Mirror: clang/test/Contracts/p4451-lookup-templates.cpp and
// p4451-r1-errors.cpp (X6) in the llvm_llvm-project fork.
#include <contracts>
namespace scl = std::contracts::controls;
using namespace std::contracts::controls;
template <class T> void f(T enforced) pre<enforced>(true);
template <class T> struct S {
  void g() pre<enforced>(true);
  void ok() pre<scl::enforced>(true);
  static constexpr int enforced = 0;
};
void use() { f(1); f(2.0); S<int> s; s.g(); s.ok(); S<long> t; t.g(); }
template <class T> void h(T x) {
  int enforced = 0;
  contract_assert<enforced>(x > 0);
}
void use2() { h(1); h(2L); }
template <class T>
void x6a(T x) pre<T::enforced>(x > 0);
template <class T> struct X6 {
  void g() pre<T::N::enforced>(true);
  void h() { contract_assert<T::enforced>(true); }
};
template <auto enforced>
void x6n() pre<enforced>(true);
template <class T>
auto x6p(T x) post<T::enforced>(r : r > 0) { return x; }
