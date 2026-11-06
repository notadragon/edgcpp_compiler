//remark:contracts: a generic lambda's own preconditions and postconditions name its captures
//type:fn
//match_regex:line 21: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 24: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 28: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 29: error: "i" .* cannot be implicitly captured only for contract assertions; capture it explicitly
//match_regex:line 35: error: "j" .* cannot be implicitly captured only for contract assertions; capture it explicitly
// P2900 [expr.prim.lambda.capture]: the predicates of a generic lambda's
// function contract specifiers name its captures, as on any lambda (the
// first lines are well-formed; EDG-50); and an implicit capture made only
// for them is ill-formed ([expr.prim.lambda.closure]), also on a generic
// lambda, and on a lambda in a template.  (A pack capture appears in
// sema/contract_only_capture.)
int f() { int t = 1; auto l = [=](auto u) pre(u > t) { return u + t; }; return l(2); }
template <class T> int g(T x) {
  auto l = [=](auto u) pre(u > x) { return u + x; };
  return l(2);
}
int h() { return g(1); }
void own(int i) {
  auto p1 = [=](auto a) pre(i > a) {};                   // Error
  auto p2 = [=](auto a) pre(i > a) { return i; };        // OK
  auto p3 = [i](auto a) pre(i > a) {};                   // OK
  auto p4 = [=](const auto a) post(i > a) {};            // Error
  p1(0); p2(0); p3(0); p4(0);
}
template <class T> void tg(T i) {
  auto q1 = [=] pre(i > 0) {};                           // Error
  auto q2 = [=](auto a) pre(i > a) {};                   // Error
  auto q3 = [=](auto a) pre(i > a) { return i; };        // OK
  q1(); q2(0); q3(0);
}
template void tg(int);
void nested(int j) {
  auto r1 = [=](auto a) { return [=](auto b) pre(b > j) { return a + b; }; };  // Error
  auto r2 = [=](auto a) { return [=](auto b) pre(b > j) { return a + b + j; }; };
  r1(0)(1); r2(0)(1);
}
