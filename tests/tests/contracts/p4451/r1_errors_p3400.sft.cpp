//remark:contracts: r1_errors under --contracts_p3400 alone and with --contracts_p4451: the P4451 check steps aside and each specifier is a P3400 label, accepted where it is one (an unqualified enforced finds the standard label through <contracts>'s contract_control using-directive, a copy of the label is a label) and otherwise rejected by P3400's own checks; recorded so
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--c++26 --contracts_p3400:--c++26 --contracts_p3400 --contracts_p4451
// Mirror: clang/test/Contracts/p4451-r1-errors.cpp (its p3400 runs) in the
// llvm_llvm-project fork.
#include <contracts>
void x1(int x) pre<enforced>(x > 0);
namespace x2v {
constexpr int enforced = 0;
void f() pre<enforced>(true);
}
namespace x2f {
void enforced();
void f() pre<enforced>(true);
}
namespace x2t {
struct enforced {};
void f() pre<enforced>(true);
}
namespace x2m {
using namespace std::contracts::controls;
struct S {
  void f() pre<enforced>(true);
  static constexpr int enforced = 0;
};
}
namespace x2a {
namespace scl = std::contracts::controls;
struct S {
  void f() pre<scl::enforced>(true);  // OK in GNU mode
  using scl = int;
};
}
void x4(int x) pre<(std::contracts::controls::enforced)>(x > 0);
void x4e(int x) pre<>(x > 0);
void x4a(int x) { contract_assert<>(x > 0); }
struct L { using assertion_control_object = L; };
constexpr L lbl{};
void x4l(int x) pre<lbl | lbl>(x > 0);
namespace x4o {
using namespace std::contracts::controls;
void f() pre<enforced | enforced>(true);
void g() pre<std::contracts::controls::enforced + 0>(true);
}
constexpr auto t = std::contracts::controls::enforced;
void x5(int x) pre<t>(x > 0);
struct Q {
  static constexpr auto enforced = std::contracts::controls::enforced;
};
void x5q(int x) pre<Q::enforced>(x > 0);
void x5l(int x) pre<lbl>(x > 0);
void x7(int x) pre<std::contracts::controls::enforce>(x > 0);
namespace x8a { constexpr int enforced = 0; }
void x8(int x) {
  using namespace x8a;
  using namespace std::contracts::controls;
  contract_assert<enforced>(x > 0);
}
namespace x8b { constexpr int enforced = 0; }
namespace x8n {
using namespace x8b;
using namespace std::contracts::controls;
void f() pre<enforced>(true);
}
namespace xp {
using namespace std::contracts::controls;
class P {
  void f() pre<enforced>(true);  // a private member: no access error
  static constexpr int enforced = 0;
};
}
