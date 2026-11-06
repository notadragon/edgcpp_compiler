//remark:contracts: an exception from the evaluation of a predicate is a violation, which quick_enforce handles by trapping (EDG-7)
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DTHROW=0;rp:-DTHROW=1;rn:-DTHROW=2;rn:-DTHROW=3;rn
//match_regex:^start$
// (No system headers: also runs under the C-generating back end.)  The
// exception never reaches the caller's handler: "caught" is never printed.
// A predicate that catches its own exception holds, and a local of a
// function whose postcondition might throw is destroyed once.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
bool check(int which) {
  if (which == THROW) throw 1;
  return true;
}
bool absorbed() {
  try { throw 2; } catch (int) {}
  return true;
}
struct D { ~D() { say("destroyed"); } };
int f(int x) pre(check(1)) { return x; }
int g(int x) { contract_assert(check(2)); return x; }
int h(const int x) post(check(3) && absorbed()) { D d; return x; }
int main() {
  say("start");
  try {
    f(1);
    g(2);
    h(3);
  } catch (...) {
    say("caught");
  }
  say("end");
}
