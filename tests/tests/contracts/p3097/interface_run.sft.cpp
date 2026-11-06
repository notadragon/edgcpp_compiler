//remark:contracts: P3097: a virtual call checks the statically chosen function's contract assertions around the final overrider's, also with the C-generating back end (EDG-71), passing the arguments on as they are
//options_all:--contracts_p3097 --contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn:-DBAD=2;rn
//match_regex:^start$
// (No system headers: also runs under the C-generating back end.)  An
// argument passed by value is copied once (by the caller), whether the call
// is virtual or not, and a class result is returned once.
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
bool note(const char *s) { say(s); return true; }
int copies = 0;
struct Arg {
  int v;
  Arg(int x) : v(x) {}
  Arg(const Arg &o) : v(o.v) { ++copies; }
};
struct Res {
  int v;
  Res(int x) : v(x) {}
  Res(const Res &o) : v(o.v) { ++copies; }
};
struct B {
  virtual int f(const int x) pre (note("B::f pre"))
    post (r: note("B::f post") && r == x) { return x; }
  virtual int g(Arg a) pre (note("B::g pre") && a.v > BAD - 1)
    { return a.v; }
  virtual Res h(const int x) post [old = x] (r: note("B::h post") && r.v == old)
    { return Res(x); }
  virtual B *self() post (r: note("B::self post") && r != nullptr) { return this; }
  virtual ~B() {}
};
struct Other { int o = 0; virtual ~Other() {} };
struct D : Other, B {
  int f(const int x) override pre (note("D::f pre")) { return x; }
  int g(Arg a) override pre (note("D::g pre")) { return a.v; }
  Res h(const int x) override { return Res(x + (BAD == 2)); }
  D *self() override { return this; }
};
int main() {
  say("start");
  B b; D d;
  B *pb = &b, *pd = &d;
  say("-- B via B*"); pb->f(1);
  say("-- D via B*"); pd->f(1);
  say("-- qualified"); pd->B::f(1);
  copies = 0;
  pd->g(Arg(BAD == 1 ? 0 : 1));
  printf("arg copies %d\n", copies); fflush(0);
  copies = 0;
  Res r = pd->h(3);
  printf("result copies %d value %d\n", copies, r.v); fflush(0);
  say(pd->self() == static_cast<B *>(&d) ? "self ok" : "self wrong");
  say("end");
}
