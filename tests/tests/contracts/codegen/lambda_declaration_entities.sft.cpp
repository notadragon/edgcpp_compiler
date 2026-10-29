//remark:contracts: checks of preconditions and postconditions whose lambdas capture the function's parameters, this and the result name, under quick_enforce
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn:-DVIOLATE=4;rn:-DVIOLATE=5;rn:-DVIOLATE=6;rn:-DVIOLATE=7;rn
// Also runs under the C-generating back end, which supports quick_enforce:
// there the lambdas capture the parameter variables, "this" and the variable
// holding the result.  (No system headers.)
extern "C" int puts(const char *);

struct S {
  int m;
  int f(int x) pre([this] { return m > 0; }()) { return m + x; }
  int g(int x) pre([&] { return m > x; }());
  int h(int x) pre([*this] { return m > 1; }()) { return x; }
};
int S::g(int x) { return m - x; }
int f(int x, int y) pre([&] { return x < y; }()) { return y - x; }
int g(int x) pre([x] { return x != 4; }()) { return x; }
int h(int &x) pre([&] { return ++(int &)x, true; }()) { return x; }
int r(const int x) post(v: [&v, x] { return v > x; }()) {
  return VIOLATE == 6 ? x : x + 1;
}

// A lambda's own precondition, whose lambda captures its parameter.
auto lam = [](int x) pre([x] { return x != 7; }()) { return x; };

int main() {
  S s{1};
  int k = 0;
  if (VIOLATE == 0) {
    s.f(1); s.g(0); f(1, 2); g(1); r(1);
    S{2}.h(0);
    lam(1);
    if (h(k) != 1) return 1;   // The lambda modified k through the reference.
  }
  if (VIOLATE == 1) S{0}.f(1);
  if (VIOLATE == 2) s.g(1);
  if (VIOLATE == 3) f(2, 1);
  if (VIOLATE == 4) g(4);
  if (VIOLATE == 5) s.h(0);
  if (VIOLATE == 6) r(5);
  if (VIOLATE == 7) lam(7);
  puts("end");
  return 0;
}
