namespace obs {
  constexpr int f(int x) pre(x > 0) { return x; }
}
constexpr int g(int x) pre(x > 0) { return x; }
