//remark:contracts: P3098: pack captures, "...c = xs" and "xs...", capture each element when the call starts, also with the C-generating back end (EDG-76)
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn:-DBAD=2;rn
//match_regex:^start$
// (No system headers: also runs under the C-generating back end.)  The
// bodies change the parameters; the captures keep their values at the call.
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
bool note(const char *s) { say(s); return true; }
template<class... T> bool positive(T... xs) { return (true && ... && (xs > 0)); }
template<class... T> int sum(T... xs) { return (0 + ... + xs); }
template<class... T> void clear(T... xs)
  post [xs...] (positive(xs...) && BAD != 1) { ((xs = 0), ...); }
template<class... T> int dbl(T... xs)
  post [...d = xs * 2] (r: r == sum(d...) + (BAD == 2)) {
  int r = 2 * sum(xs...); ((xs = 0), ...); return r; }
template<class... T> int count(int n, T... xs)
  post [m = n, ...c = xs, xs...]
       (r: r == m + int(sizeof...(c)) && sum(c...) == sum(xs...)) {
  int r = n + int(sizeof...(xs)); n = 0; ((xs = 0), ...); return r; }
// Class elements are copied and destroyed after the check.
struct Box {
  int v;
  Box(int x) : v(x) {}
  Box(const Box &o) : v(o.v) { say("copied"); }
  ~Box() { say("destroyed"); }
};
template<class... T> void boxes(const T &... bs)
  post [...c = bs] (note("check") && sum(c.v...) == 3) {}
int main() {
  say("start");
  clear(1, 2, 3);
  clear();
  dbl(1, 2, 3);
  dbl();
  count(1, 2, 3);
  count(1);
  Box a(1), b(2);
  boxes(a, b);
  say("end");
}
