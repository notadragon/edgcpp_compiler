//remark:contracts: P4451R2 [basic.contract.control] under --contracts_p4451 alone: each way of not naming the standard library declaration of std::contracts::controls::enforced is one error -- lookup finds nothing (X1, also a name std::contracts::controls does not declare, X7), another declaration (X2: a variable, a function, a type, a later member with a using-directive in scope; X5: another variable or a class member copying the label), an ambiguous result (X8), something that is not an id-expression (X4, an empty specifier, a P3400-style label object, enforced | enforced, an expression), or a dependent name (X6, see lookup_templates); a later private member gives no access error; a namespace alias that a later member type alias hides is accepted, as by g++ (GNU mode: lookup before :: skips a non-class type)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--c++26 --contracts_p4451
//match_regex:line 29: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds nothing
//match_regex:line 32: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 36: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 40: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 45: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 57: error: assertion-control-specifier must name std::contracts::controls::enforced; it is not an id-expression naming a declaration
//match_regex:line 58: error: assertion-control-specifier must name std::contracts::controls::enforced; it is not an id-expression naming a declaration
//match_regex:line 59: error: assertion-control-specifier must name std::contracts::controls::enforced; it is not an id-expression naming a declaration
//match_regex:line 62: error: assertion-control-specifier must name std::contracts::controls::enforced; it is not an id-expression naming a declaration
//match_regex:line 65: error: assertion-control-specifier must name std::contracts::controls::enforced; it is not an id-expression naming a declaration
//match_regex:line 66: error: assertion-control-specifier must name std::contracts::controls::enforced; it is not an id-expression naming a declaration
//match_regex:line 70: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 74: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 75: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
//match_regex:line 77: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds nothing
//match_regex:line 83: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup is ambiguous
//match_regex:line 89: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup is ambiguous
//match_regex:line 94: error: assertion-control-specifier must name std::contracts::controls::enforced; name lookup finds another declaration
// Mirror: clang/test/Contracts/p4451-r1-errors.cpp in the
// llvm_llvm-project fork; r1_errors_p3400 is this test under
// --contracts_p3400 alone and with both.  X3 (a qualified name before
// <contracts>) is imported_p4451/gcc/p4451-precedes.
#include <contracts>
// X1
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
// X4 and other forms that are not an id-expression.
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
// X5
constexpr auto t = std::contracts::controls::enforced;
void x5(int x) pre<t>(x > 0);
struct Q {
  static constexpr auto enforced = std::contracts::controls::enforced;
};
void x5q(int x) pre<Q::enforced>(x > 0);
void x5l(int x) pre<lbl>(x > 0);
// X7
void x7(int x) pre<std::contracts::controls::enforce>(x > 0);
// X8
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
