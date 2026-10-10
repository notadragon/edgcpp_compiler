//remark:contracts: P3098: postcondition captures are checked at run time, also with the C-generating back end (EDG-75): initialized in declaration order with the preconditions, destroyed after the postconditions are checked
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn:-DBAD=2;rn
//match_regex:^start$
// (No system headers: also runs under the C-generating back end.)  The
// lines printed show the order: a local is destroyed, then the
// postcondition is checked, then the capture is destroyed.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
struct Box {
  int v;
  Box(int x) : v(x) {}
  Box(const Box &o) : v(o.v) { say("capture copied"); }
  ~Box() { say("capture destroyed"); }
};
struct Local { ~Local() { say("local destroyed"); } };
bool note(const char *s) { say(s); return true; }
int inc(int &x) post [old = x] (x == old + 1 + (BAD == 1)) { ++x; return x; }
int grow(Box &b) post [old = b] (note("check") && b.v > old.v + (BAD == 2)) {
  Local l;
  b.v += 1;
  return b.v;
}
bool order(const char *s) { return note(s); }
void interleave(const int x)
  pre (order("pre 1"))
  post [c = (note("capture"), x)] (order("post") && c == x)
  pre (order("pre 2"))
{}
int main() {
  say("start");
  int i = 1;
  inc(i);
  Box b(1);
  grow(b);
  interleave(3);
  say("end");
}
