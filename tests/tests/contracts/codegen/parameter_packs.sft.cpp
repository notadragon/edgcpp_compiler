//remark:contracts: run-time checks of uses of a function parameter pack's elements, and of a parameter after a pack
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn:-DVIOLATE=4;rn
// Also runs under the C-generating back end, whose checks are built from a
// copy of the predicate in which each use of a parameter becomes the
// definition's parameter variable: the elements of an expanded parameter
// pack share the pack's parameter number (EDG-47), so each use must still
// become its own element, and a parameter after the pack its own variable.
// (No system headers: the C-generating configuration cannot find them.)
extern "C" int puts(const char *);

template<class... Ts> int f(Ts... a, int b) pre(b > 0) { return b; }
template<class... P> int w(double t, P... p) pre(((p < t) && ...)) {
  return 0;
}
template<class... P> int ix(P... p) pre(p...[1] > 0) { return 0; }
template<class... P> int y(P... p, const int n) post(r: r == n) {
  return (p + ...);
}

int main() {
  if (VIOLATE == 0) {
    f<int, int>(-1, -1, 3);
    w(5.0, 1, 2);
    ix(-1, 1);
    y<int, int>(1, 2, 3);
  }
  if (VIOLATE == 1) f<int, int>(1, 1, -3);
  if (VIOLATE == 2) w(5.0, 1, 9);
  if (VIOLATE == 3) ix(1, -1);
  if (VIOLATE == 4) y<int, int>(1, 2, 4);
  puts("end");
  return 0;
}
