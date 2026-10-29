//remark:contracts: an immediate invocation in the unselected operand of a constant conditional expands its source location default arguments
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--c++26
// Not contracts-specific (an upstream fix, EDG-66), but P3290 depends on it:
// a contract-integrated assert(1 == 1) expands to
//   static_cast<bool>(1 == 1) ? void(0)
//     : __cxa_handle_cassert_violation(#cond, std::source_location::current())
// The unselected operand of a conditional whose condition is a constant is
// potentially evaluated, so a consteval call in it is still an immediate
// invocation ([expr.const]) and its default arguments, including source
// location builtins, are evaluated at the call site.
#include <source_location>
using SL = std::source_location;
void g(SL);
consteval unsigned line(unsigned l = __builtin_LINE()) { return l; }
consteval unsigned line_is_20(unsigned l = __builtin_LINE()) {
  return l == 20 ? l : throw 0;
}
unsigned u0 = 1 ? 0u : line_is_20();  // OK, folded for this line (20)
constexpr unsigned ln(SL s = SL::current()) { return s.line(); }
constexpr bool T = true;
void f(int x) {
  x == 1 ? (void)0 : g(SL::current());  // OK
  1 == 1 ? (void)0 : g(SL::current());  // OK
  true ? (void)0 : g(SL::current());    // OK
  T ? (void)0 : g(SL::current());       // OK
  false ? g(SL::current()) : (void)0;   // OK
  (void)(1 ? 0u : line());              // OK
  (void)(0 ? line() : 0u);              // OK
  (void)(1 ? 0u : ln());                // OK
}
constexpr unsigned cf() {
  return 1 ? SL::current().line() : SL::current().line();
}
static_assert(cf() == 34);
template <int N> void t() {
  N ? (void)0 : g(SL::current());       // OK
}
template void t<1>();
constexpr unsigned a = (0 ? 0u : SL::current().line()) +
                       (1 ? 0u : SL::current().line());
static_assert(a == 41);
constexpr unsigned b = 0 ? 1u
                         : ln();
static_assert(b == 45);
