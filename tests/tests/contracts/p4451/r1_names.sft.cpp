//remark:contracts: P4451R2 [basic.contract.control]: every way of naming the standard library declaration of std::contracts::controls::enforced by ordinary name lookup is accepted and makes the assertion terminating (enforced under a configured ignore, through g++, which gets the specifier back as <::std::contracts::controls::enforced>): qualified (R1), from the global namespace (R2), namespace aliases (R3), using-directives at namespace and block scope and through a namespace that has one (R4), using-declarations (R5), a user namespace's using-declaration named qualified, a member function whose class later declares members named std, terminating and scl (R7, complete-class context), a block-scope variable named std, a later member class named std (with ::std::), non-dependent names in templates, instantiated (R8), and redeclarations spelled differently; the same under --contracts_p3400 alone and with both (E173)
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options:--contracts_p4451 --contract_evaluation_semantic=ignore:--contracts_p3400 --contract_evaluation_semantic=ignore:--contracts_p3400 --contracts_p4451 --contract_evaluation_semantic=ignore
//use_system_includes: true
//linker_options: -lcontracts
// Mirror: clang/test/Contracts/p4451-r1-names.cpp in the llvm_llvm-project
// fork.
#include <contracts>
#include <cstdlib>
static int handled;
void handle_contract_violation(const std::contracts::contract_violation &v)
{ if (v.semantic() != std::contracts::evaluation_semantic::enforce) std::abort(); ++handled; throw handled; }
int r1(int x) pre<std::contracts::controls::enforced>(x > 0)
    post<std::contracts::controls::enforced>(r : r > 1) {
  contract_assert<std::contracts::controls::enforced>(x != 5);
  return x;
}
int r2(int x) pre<::std::contracts::controls::enforced>(x > 0) { return x; }
namespace scl = std::contracts::controls;
namespace sc = std::contracts;
int r3(int x) pre<scl::enforced>(x > 0) post<sc::controls::enforced>(r : r > 1) { return x; }
namespace r4ns {
using namespace std::contracts::controls;
int f(int x) pre<enforced>(x > 0) { return x; }
namespace inner { int g(int x) pre<enforced>(x > 0) { return x; } }
}
int r4q(int x) pre<r4ns::enforced>(x > 0) { return x; }
int r4b(int x) {
  using namespace std::contracts::controls;
  contract_assert<enforced>(x > 0);
  auto l = [](int y) pre<enforced>(y > 0) { return y; };
  return l(x);
}
namespace r5ns {
using std::contracts::controls::enforced;
int f(int x) pre<enforced>(x > 0) { return x; }
}
int r5q(int x) pre<r5ns::enforced>(x > 0) { return x; }
int r5b(int x) {
  using std::contracts::controls::enforced;
  contract_assert<enforced>(x > 0);
  return x;
}
struct R7 {
  int f(int x) pre<std::contracts::controls::enforced>(x > 0) { return x; }
  int g(int x) pre<scl::enforced>(x > 0);
  static constexpr int std = 0;
  static constexpr int enforced = 0;
  int scl = 0;
};
int R7::g(int x) pre<scl::enforced>(x > 0) { return x; }
struct R7c {
  int f(int x) pre<::std::contracts::controls::enforced>(x > 0) { return x; }
  struct std {};
};
int r7b(int x) {
  int std = x;
  contract_assert<std::contracts::controls::enforced>(std > 0);
  return std;
}
template <class T>
T r8f(T x) pre<std::contracts::controls::enforced>(x > 0) post<scl::enforced>(r : r > 0) {
  using namespace std::contracts::controls;
  contract_assert<enforced>(x != 5);
  return x;
}
template <class T> struct R8 {
  T m(T x) pre<scl::enforced>(x > 0) { return x; }
  T d(T x) pre<::std::contracts::controls::enforced>(x > 0);
  T a(T x) { contract_assert<r4ns::enforced>(x > 0); return x; }
};
template <class T> T R8<T>::d(T x) pre<::std::contracts::controls::enforced>(x > 0) { return x; }
auto r8l = [](auto x) pre<r5ns::enforced>(x > 0) { return x; };
int rd(int x) pre<scl::enforced>(x > 0);
int rd(int x) pre<std::contracts::controls::enforced>(x > 0) { return x; }
#define CHECK(E) do { try { (void)(E); std::abort(); } catch (int) {} } while (0)
int main() {
  CHECK(r1(0)); CHECK(r1(1)); CHECK(r1(5)); CHECK(r2(0)); CHECK(r3(0)); CHECK(r3(1));
  CHECK(r4ns::f(0)); CHECK(r4ns::inner::g(0)); CHECK(r4q(0)); CHECK(r4b(0));
  CHECK(r5ns::f(0)); CHECK(r5q(0)); CHECK(r5b(0));
  R7 s; CHECK(s.f(0)); CHECK(s.g(0)); CHECK(r7b(0)); R7c c; CHECK(c.f(0));
  CHECK(r8f(0)); CHECK(r8f(5L));
  R8<int> i; CHECK(i.m(0)); CHECK(i.d(0)); CHECK(i.a(0));
  CHECK(r8l(0)); CHECK(r8l(0.0)); CHECK(rd(0));
  if (handled != 25) std::abort();
  r1(2); r2(2); r3(2); r4ns::f(2); r4ns::inner::g(2); r4q(2); r4b(2);
  r5ns::f(2); r5q(2); r5b(2); s.f(2); s.g(2); r7b(2); c.f(2); r8f(2); r8f(2L);
  i.m(2); i.d(2); i.a(2); r8l(2); r8l(2.0); rd(2);
  return handled == 25 ? 0 : 1;
}
