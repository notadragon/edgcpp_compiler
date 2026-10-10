//remark:typeid of a glvalue of polymorphic class type evaluates its operand, even where its dynamic type is known
//require:DO_IL_LOWERING 1
//options_all:--clang --clang_version 190100
//type:rp
// Not contracts-specific (an upstream fix, EDG-58): the C-generating back
// end skips the run-time lookup when the type of the complete object is
// known, but keeps the operand's side effects.  (Clang mode, where a bare
// std::type_info declaration suffices: no system headers here.)
namespace std { class type_info { }; }
extern "C" int puts(const char *);
int calls;
int f() { return ++calls; }
struct B { virtual void v() {} };
struct D : B { };
struct N { };
int main() {
  D d;
  N n;
  (void)typeid(f(), d);   // Evaluated: f is called
  (void)typeid(f(), n);   // Unevaluated: f is not called
  if (calls != 1) return 1;
  puts("end");
}
