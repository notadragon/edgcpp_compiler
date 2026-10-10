//remark:contracts: a postcondition's result name in a function with a deduced return type is checked at run time, also with the C-generating back end (EDG-9)
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn:-DBAD=2;rn:-DBAD=3;rn:-DBAD=4;rn:-DBAD=5;rn
//match_regex:^start$
// (No system headers: also runs under the C-generating back end.)  The
// postconditions are checked in declaration order (a, b, c), with a
// capture, on a member naming this, on a reference result, in a template's
// instance and a lambda; BAD makes one of them fail.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
bool note(const char *s) { say(s); return true; }
auto f(const int x) post (note("a")) post (r: note("b") && r == x + (BAD == 1))
  post (note("c")) {
  if (x > 10) return x;
  return x;
}
auto cap(int &x) post [old = x] (r: r == old + 1 + (BAD == 2)) {
  ++x;
  return x;
}
struct S {
  int m = 4;
  auto get() const post (r: r == m + (BAD == 3)) { return m; }
};
int g = 7;
decltype(auto) ref() post (r: &r == &g && (BAD != 4)) { return (g); }
template <class T> auto twice(const T t) post (r: r == t * 2 + (BAD == 5)) {
  return t * 2;
}
int main() {
  say("start");
  f(3);
  int i = 1;
  cap(i);
  S s;
  s.get();
  ref();
  twice(4);
  auto l = [](const int y) post (r: r == y) { return y; };
  l(2);
  say("end");
}
