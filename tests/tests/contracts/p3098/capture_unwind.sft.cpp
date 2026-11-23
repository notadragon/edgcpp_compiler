//remark:contracts: P3098: a postcondition capture is destroyed when an exception leaves the function body, also with the C-generating back end (EDG-86), and once on a normal return
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:;rp
//match_regex:^(caught|handler|end)$
// (No system headers: also runs under the C-generating back end.)  Each
// capture is copied, then destroyed once: before "caught" when the body
// throws, after the check on a normal return.
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
bool may_throw(int v) { if (v < 0) throw 2; return true; }
void f(const Box &b, const int n) post [c = b] (c.v == 1) {
  Local l;
  if (n == 1) throw 1;
  if (n == 2) return;
}
// A capture whose initializer might throw (so a flag guards it).
void g(const Box &b, const int n) post [c = b, d = may_throw(n)] (d) {
  if (n == 1) throw 1;
}
// A function-try-block whose handler rethrows, and one whose handler returns.
void h(const Box &b, const int n) post [c = b] (c.v == 1)
try { if (n > 0) throw 1; } catch (int) { say("handler"); if (n == 1) throw; }
int main() {
  {
    Box b(1);
    try { f(b, 1); } catch (int) { say("caught"); }
    f(b, 2);
    f(b, 3);
    try { g(b, 1); } catch (int) { say("caught"); }
    g(b, 0);
    try { h(b, 1); } catch (int) { say("caught"); }
    h(b, 2);
  }
  say("end");
}
