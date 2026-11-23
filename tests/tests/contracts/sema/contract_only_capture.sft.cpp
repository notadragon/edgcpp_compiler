//remark:contracts: an implicit capture made only for contract assertions is ill-formed
//type:fn
//match_regex:line 34: error: "this" cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 38: error: "this" cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 43: error: "this" cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 44: error: "this" cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 48: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 49: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 53: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 54: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 56: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 57: error: "[^"]*::y" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 58: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 61: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 63: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 73: error: "a" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 77: error: "x" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 83: error: "ts" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 93: error: "this" cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 98: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 101: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 102: error: "y" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 108: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 109: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
// An implicit capture made only because contract assertions of the lambda
// name the entity (a local, or "this") is ill-formed (P2900
// [expr.prim.lambda.closure]), including for a lambda within a contract
// assertion of the lambda; an error is given once, for the innermost such
// capture.  A constant not odr-used is not captured.  The same holds for
// a lambda's own preconditions and postconditions.
struct S {
  int m;
  int get() const { return m; }
  void h() { auto l = [&] { contract_assert(m > 0); }; l(); }       // Error
  void h2() { auto l = [&] { contract_assert(m > 0); return m; }; l(); }
  void h3() { auto l = [this] { contract_assert(m > 0); }; l(); }
  void h4() { auto l = [=, this] { contract_assert(m > 0); }; l(); }
  void h5() { [&](auto a) { contract_assert(get() > a); }(1); }    // Error
  void h6() { auto l = [&] { contract_assert(get() > 0); get(); }; l(); }
};
struct T {
  int m = 0;
  int f = [this] { [&] { contract_assert(m > 0); }(); return 1; }(); // Error
  int g = [&] { contract_assert(this->m > 0); return 1; }();       // Error
};
void f(int x) {
  const int n = 5;
  auto l1 = [=] { contract_assert(x > 0); };                       // Error
  auto l2 = [&](auto a) { contract_assert(x > a); return a; };     // Error
  auto l3 = [=] { contract_assert(x > 0); (void)x; };              // OK
  auto l4 = [x] { contract_assert(x > 0); };                       // OK
  auto l5 = [=] { contract_assert(n > 0); };                       // OK
  auto l6 = [=] { return [=] { contract_assert(x > 0); }; };       // Error
  auto l7 = [=] { contract_assert([=] { return x > 0; }()); };     // Error
  auto l8 = [=] { return [x] { contract_assert(x > 0); }; };       // OK
  auto l9 = [=] { contract_assert([x] { return x > 0; }()); };     // Error
  auto l10 = [y = x] { [=] { contract_assert(y > 0); }(); };       // Error
  auto l11 = [&] { contract_assert(x > 0); contract_assert(x < 9); };  // Error
  auto l12 = [&](auto a) { contract_assert(x > a); return a + x; }; // OK
  auto l13 = [=] {
    contract_assert([=] { contract_assert(x > 0); return x > 1; }());  // Error
  };
  auto l14 = [&] { [&] { contract_assert(x > 0); }(); (void)x; };  // Error
  auto l15 = [&] { [&] { (void)x; }(); contract_assert(x > 0); };  // OK
  auto l16 = [&] { contract_assert(sizeof(x) > 0); };              // OK
  auto l17 = [=]() mutable { contract_assert(x > 0); x = 1; };     // OK
  l1(); l2(1); l3(); l4(); l5(); l6(); l7(); l8(); l9(); l10(); l11();
  l12(1); l13(); l14(); l15(); l16(); l17();
}
void sb() {
  struct B { int a, b; };
  auto [a, b] = B{1, 2};
  auto l = [&] { contract_assert(a > 0); };                        // Error
  l();
}
template <class T> void g(T x) {
  auto l = [=] { contract_assert(x > 0); };                        // Error
  auto m = [=] { contract_assert(x > 0); return x; };              // OK
  l(); m();
}
template void g(int);
template <class... Ts> void p(Ts... ts) {
  auto l = [=] { contract_assert(((ts > 0) && ...)); };            // Error
  auto m = [=] {
    contract_assert(((ts > 0) && ...));                             // OK
    return (ts + ...);
  };
  l(); m();
}
template void p(int, int);
struct U {
  int m;
  void g() { auto s = [&] pre(m > 0) {}; s(); }                  // Error
  void g2() { auto s = [&] pre(m > 0) { return m; }; s(); }
};
void own(int i) {
  bool y = true;
  auto p1 = [=] pre(i > 0) {};                                     // Error
  auto p2 = [i] pre(i > 0) {};                                     // OK
  auto p3 = [=] pre(i > 0) { return i; };                          // OK
  auto p4 = [=](const int a) post(i > a) {};                       // Error
  auto p5 = [=] pre([=] { return y; }()) {};                       // Error
  auto p6 = [=] pre([=] { return y; }()) { return y; };            // OK
  auto p7 = [=] pre([] {
    bool x = true;
    return [=] { return x; }();                                    // OK
  }()) {};
  auto p8 = [=] { return [=] pre(i > 0) {}; };                     // Error
  auto p9 = [=] { (void)i; return [=] pre(i > 0) {}; };            // Error
  p1(); p2(); p3(); p4(1); p5(); p6(); p7(); p8(); p9();
}
