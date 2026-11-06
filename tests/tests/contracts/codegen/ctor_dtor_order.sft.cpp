//remark:contracts: a constructor's preconditions precede its mem-initializers, a destructor's postconditions follow its member destructions
//require:DO_IL_LOWERING 1
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=1;rn:-DVIOLATE=2;rn
//match_regex:^start$
// With the C-generating back end (EDG-13): the precondition traps before
// "member initialized" is printed, and the postcondition only after
// "member destroyed".  (No system headers there.)
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
int note(int a) { say("member initialized"); return a; }
bool ok = false;
struct M { ~M() { say("member destroyed"); } };
struct S {
  int v;
  M m;
  S(int a) pre(a > 0) : v(note(a)) {}
  ~S() post(ok) {}
};
int main() {
  say("start");
  if (VIOLATE == 1) { S s(0); }
  if (VIOLATE == 2) { S s(1); }
}
