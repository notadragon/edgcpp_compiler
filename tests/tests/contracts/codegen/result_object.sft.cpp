//remark:contracts: postconditions see the result object itself, after the locals are destroyed
//require:DO_IL_LOWERING 1
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn:-DBAD=2;rn:-DBAD=3;rn:-DBAD=4;rn
//match_regex:^start$
// With the C-generating back end (EDG-5, EDG-6): a postcondition names the
// object returned, for a reference, a class that is not trivially copyable
// and a trivially copyable class, and is checked after the function's local
// variables are destroyed, at every return and on flowing off the end of a
// void function.  Each postcondition here would fail otherwise; BAD
// makes one of them fail, to show that it is checked.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
int g = 0;
struct NT {
  int v;
  NT *self;
  NT(int x) : v(x), self(this) {}
  NT(const NT &o) : v(o.v), self(this) {}
  ~NT() {}
};
struct Guard { int &r; ~Guard() { r = 42; } };
int &ref(int &x) post(r : (&r == &x) != (BAD == 1)) { return x; }
const int &cref(const int &x) post(r : &r == &x && r == 5) { return x; }
NT make(const int x) post(r : r.self == &r && r.v == x + (BAD == 2)) {
  if (x > 5) return NT(x);
  NT n(x);
  return n;
}
int locals(int &p) post(p == 42) { Guard gd{p}; return 1; }
int multi(const int x) post(r : r == x * 2) post(g == 42) {
  if (x == 1) { g = 42; return 2; }
  if (x == 2) { Guard gd{g}; return 4; }
  return x * 2;
}
void v(int x) post(g == 7 + (BAD == 4)) { if (x) { g = 7; return; } g = 7; }
struct POD { int a, b; };
POD pod(const int x) post(r : r.a == x && r.b == (BAD == 3)) { return POD{x, 0}; }
struct C {
  int m;
  C(const int x) post(m == x) : m(x) {}
  int &get() post(r : &r == &m) { return m; }
};
int main() {
  say("start");
  int x = 0, five = 5;
  ref(x);
  cref(five);
  make(3); make(9);
  int p = 0; locals(p);
  multi(1); multi(2); multi(3);
  v(0); v(1);
  pod(4);
  C c(3);
  c.get();
  say("end");
}
