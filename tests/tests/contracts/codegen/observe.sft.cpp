//remark:contracts: checks of preconditions, postconditions and contract_assert under observe
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
// Every violation reaches the default violation handler (whose report goes
// to stderr), and evaluation continues.  The program's own output also goes
// to stderr, so that the two stay in order.
#include <cstdio>
#include <string>

#define MARK(n) std::fprintf(stderr, "-- %d\n", n)

int pres(int x) pre(x > 0) pre(x < 10) { return x; }
int posts(const int x) post(r: r > x) post(x != 7) {
  if (x > 5) return x;
  return x + 1;
}
void post_void(const int x) post(x != 3) {
  if (x == 1) return;
}
void post_void_expr(const int x) post(x != 4) {
  return (void)std::fprintf(stderr, "post_void_expr(%d)\n", x);
}
struct P { int a, b; };
P post_class(const int a) post(r: r.b == a + 1) { return P{a, a}; }
P post_class_var(const int a) post(r: r.a == a) {
  P p{a + 1, 0};
  return p;
}
int declared(int x) pre(x != 0);
int declared(int x) { return x; }
bool temps(const std::string& s) pre(s + "!" != "a!") { return true; }
void asserts(int z) {
  contract_assert(z > 0);
  if (z < -5) contract_assert(z > -6);
}

int main() {
  auto lam = [](const int y) -> int pre(y != 0) post(r: r == 2 * y) {
    return y;
  };
  MARK(1); pres(5);              // no violation
  MARK(2); pres(0); pres(20);    // first, then second precondition
  MARK(3); posts(1); posts(6);   // second return, then first
  MARK(4); posts(7);             // second postcondition
  MARK(5); post_void(1); post_void(3);  // explicit return, then end of body
  MARK(6); post_void_expr(4);    // after the expression is evaluated
  MARK(7); post_class(1); post_class_var(2);
  MARK(8); declared(0);          // contract on the first declaration only
  MARK(9); temps("a");
  MARK(10); asserts(-7);
  MARK(11); lam(0);
  MARK(12);
}
