//remark:contracts: EDG-88: with the C-generating back end, a virtual call of a C-variadic function does not check the statically chosen function's contract assertions (P3097)
//require:DO_IL_LOWERING 1
//options_all:--contracts_p3097 --contract_evaluation_semantic=quick_enforce
//options:;rp
//match_regex:^(start|end)$
// The recording is the current, wrong behavior: the call should trap on
// B::f's precondition (as our GCC does) before "end" is printed.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
struct B { virtual int f(int n, ...) pre (n >= 0) { return n; } virtual ~B() {} };
struct D : B { int f(int n, ...) override { return n; } };
int main() {
  say("start");
  D d;
  B *p = &d;
  p->f(-1);
  say("end");
}
