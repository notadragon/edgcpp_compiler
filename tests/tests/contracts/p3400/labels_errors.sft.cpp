//remark:contracts: P3400 assertion-control specifiers are of a class type with a nested assertion_control_object type, constant, and the same on every declaration; contract_control names are found only in them
//type:fn
//options_all:--contracts_p3400 --contract_evaluation_semantic=quick_enforce
//match_regex:line 23: error: an assertion-control expression must be of a class type
//match_regex:line 24: error: type "NotLabel" does not satisfy assertion_control_object
//match_regex:line 25: error: type "Private" does not satisfy assertion_control_object
//match_regex:line 27: error: an assertion-control expression must be a constant expression
//match_regex:line 30: error: mismatched assertion-control label
//match_regex:line 32: error: mismatched assertion-control label
//match_regex:line 34: error: identifier "hidden_label" is undefined
//match_regex:line 39: error: identifier "y_label" is undefined
//match_regex:line 46: error: an assertion-control expression must be a constant expression
// (No system headers: also runs under the C-generating back end.)
struct Label { using assertion_control_object = Label; };
struct NotLabel {};
class Private { using assertion_control_object = Private; };
namespace L { inline constexpr Label hidden_label{}; }
using contract_control namespace L;
constexpr Label lbl{};
struct Valued { using assertion_control_object = Valued; int v; };
Valued runtime_val{1};

void a() pre<5>(true);                      // Error
void b() pre<NotLabel{}>(true);             // Error
void c() pre<Private{}>(true);              // Error
void d() pre<hidden_label>(true);           // OK
void e() pre<runtime_val>(true);            // Error
void f(int x) pre<lbl>(x > 0);
void f(int x) pre<lbl>(x > 0);              // OK
void f(int x) pre(x > 0);                   // Error
void g(int x) pre(x > 0);
void g(int x) pre<lbl>(x > 0);              // Error
void h() { contract_assert<lbl>(true); }    // OK
auto i = hidden_label;                      // Error
auto j = contract_control(hidden_label);    // OK
namespace Y { inline constexpr Label y_label{}; }
namespace X { using namespace Y; }
using contract_control namespace X;
auto k = y_label;                           // Error
auto m = contract_control(y_label);         // OK
namespace Z { inline constexpr Label z_label{}; }
using contract_control namespace Z;
using namespace Z;
auto n = z_label;                           // OK
const int *p = &contract_control(42);       // OK
auto q = contract_control(runtime_val);     // Error
