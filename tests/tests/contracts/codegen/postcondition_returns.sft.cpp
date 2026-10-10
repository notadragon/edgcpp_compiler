//remark:contracts: a postcondition violated at one of several returns
//require:DO_IL_LOWERING 1
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DARG=0;rp:-DARG=1;rn:-DARG=2;rn
//match_regex:^start$
// With the C-generating back end: each return reaches the postcondition
// check, which traps when it fails; "end" is printed only when it holds.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
struct D { ~D() { say("local destroyed"); } };
int f(int x) post(r : r > 0) {
  D d;
  if (x == 1) return -1;
  if (x == 2) { D e; return 0; }
  return 1;
}
int main() {
  say("start");
  f(ARG);
  say("end");
}
