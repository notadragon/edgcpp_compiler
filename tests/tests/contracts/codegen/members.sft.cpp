//remark:contracts: checks of the contract assertions of member functions under observe
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
// The contract assertions of a member function, or of a friend declared in
// a class, are a complete-class context: they can name members declared after
// them.  Members defined in and out of the class, static members,
// constructors, destructors, members of nested and local classes, and friends
// defined in and out of the class.
// A friend declared in the class and defined outside it names a member
// qualified: g++ (stock as well as ours) checks its precondition in the
// definition's scope and cannot find an unqualified "limit" (EDG and Clang
// accept it).
#include <cstdio>

#define MARK(n) std::fprintf(stderr, "-- %d\n", n)

struct S {
  int v = 1;
  int f(int x) pre(x > 0) post(r: r > v) post(r: r != later()) {
    return x + v;
  }
  int g(int x) pre(x > v);
  static int h(int x) pre(x != 0) { return x; }
  S(int a) pre(a >= 0) : v(a) {}
  ~S() pre(v != 7) {}
  int later() const { return 99; }
  struct N { void n(int q) pre(q > 5) {} };
  int i(int x) const pre(x < limit) { return x; }
  friend int fr(S &s, int x) pre(s.later() != 99) { return x; }
  friend int fo(int x) pre(x != S::limit);  // (qualified for g++; see below)
  static constexpr int limit = 10;
};
int S::g(int x) { return x; }
int fo(int x) { return x; }

int main() {
  struct L { int m(int k) pre(k != 2) { return k; } };
  MARK(1); S s(-1);              // constructor
  MARK(2); s.f(1);               // no violation
  MARK(3); s.f(-1);              // precondition, then first postcondition
  MARK(4); s.g(-2);              // declared in the class, defined outside
  MARK(5); S::h(0);
  MARK(6); S::N().n(1);
  MARK(7); s.i(10);              // names a member declared later
  MARK(8); { S t(7); }           // destructor
  MARK(9); L().m(2);
  MARK(10); fr(s, 1);            // friend defined in the class
  MARK(11); fo(10);              // friend defined outside it
  MARK(12);
}
