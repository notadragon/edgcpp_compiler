//remark:typeid of a glvalue of polymorphic class type odr-uses what it names, even where its dynamic type is known
//type:fn
//match_regex:line 13: error: an enclosing-function local variable cannot be referenced in a lambda body unless it is in the capture list
//match_regex:line 14: error: an enclosing-function local variable cannot be referenced in a lambda body unless it is in the capture list
//require:BACK_END_IS_CP_GEN_BE 1
// Not contracts-specific (an upstream fix, EDG-58): the operand is evaluated
// ([expr.typeid]/3), so a lambda must capture the variable, whether or not
// the type of the complete object is known and the run-time lookup skipped.
#include <typeinfo>
struct B { virtual ~B(); };
struct N { };
void f(B x, B &r, N n) {
  auto l1 = [] { return &typeid(x); };      // Error, x not captured
  auto l2 = [] { return &typeid(r); };      // Error, r not captured
  auto l3 = [x] { return &typeid(x); };     // OK
  auto l4 = [=] { return &typeid(x); };     // OK, captures x
  auto l5 = [] { return &typeid(n); };      // OK, unevaluated operand
  (void)l1; (void)l2; (void)l3; (void)l4; (void)l5;
}
