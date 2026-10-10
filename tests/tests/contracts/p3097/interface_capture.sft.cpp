//remark:contracts: P3097 with P3098: a capture of the statically chosen function's postcondition in a virtual call is made once and destroyed once, also when the final overrider throws, also with the C-generating back end (EDG-71)
//options_all:--contracts_p3097 --contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:;rp
//match_regex:^(caught|end)$
// (No system headers: also runs under the C-generating back end.)
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
struct Box {
  int v;
  Box(int x) : v(x) {}
  Box(const Box &o) : v(o.v) { say("capture copied"); }
  ~Box() { say("capture destroyed"); }
};
struct B {
  virtual int k(const Box &b, const int n) post [c = b] (r: c.v == r) {
    return n;
  }
  virtual ~B() {}
};
struct D : B {
  int k(const Box &b, const int n) override {
    if (n == 0) throw 1;
    return b.v;
  }
};
int main() {
  {
    Box b(1);
    D d;
    B *p = &d;
    p->k(b, 1);
    try { p->k(b, 0); } catch (int) { say("caught"); }
  }
  say("end");
}
