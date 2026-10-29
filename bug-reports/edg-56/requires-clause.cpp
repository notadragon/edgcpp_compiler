namespace n {
  constexpr int a = 1;
  constexpr int b = 2;
  constexpr int c = 0;
  constexpr int pre(int i) { return i; }
  template <typename T>
  void f() requires (a < b > pre(c));
  template <typename T> constexpr bool d = true;
  using e = char;
  template <typename T>
  void g() requires d < e >;
  template <typename T>
  void f() requires (a < b > pre(c));
}
