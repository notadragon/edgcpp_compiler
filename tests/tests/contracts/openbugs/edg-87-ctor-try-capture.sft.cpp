//remark:contracts: EDG-87: with the C-generating back end, a postcondition capture of a constructor or destructor whose body is a function-try-block is not destroyed when an exception leaves it
//require:DO_IL_LOWERING 1
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:;rp
//match_regex:^(caught|end)$
// The recording is the current, wrong behavior: "capture destroyed" should
// be printed between "handler" and "caught" (as our GCC does).
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
struct Box {
  int v;
  Box(int x) : v(x) {}
  Box(const Box &o) : v(o.v) { say("capture copied"); }
  ~Box() { say("capture destroyed"); }
};
struct S {
  S(const Box &b) post [c = b] (c.v == 1)
  try { throw 1; } catch (int) { say("handler"); }
};
int main() {
  {
    Box b(1);
    try { S s(b); } catch (int) { say("caught"); }
  }
  say("end");
}
