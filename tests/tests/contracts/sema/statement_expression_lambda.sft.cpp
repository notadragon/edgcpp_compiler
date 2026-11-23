//remark:contracts: a statement expression in a lambda's precondition or postcondition
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
//match_regex:", line 18: error: expression must be a modifiable lvalue
//match_regex:", line 19: error: expression must be a modifiable lvalue
//match_regex:", line 31: error: statement expressions are only allowed in block scope
//match_regex:", line 33: error: statement expressions are only allowed in block scope
// A lambda's predicates are scanned in its call operator (see EDG-4), so a
// GNU statement expression may appear in them, as GCC and Clang accept.  A
// parameter or capture used in it is const, as in the predicate, but a
// variable declared in it is not; the same in a template-dependent context.
// Elsewhere a predicate is outside any function, as GCC rejects.
int f(int v) {
  int c = 1;
  auto l1 = [c](int x) -> int pre(({ x > c; })) post(r: ({ r > 0; }))
                                                    { return x; };   // OK
  auto l2 = [](int x) pre(({ int y = x; ++y; y > 1; })) { return x; }; // OK
  auto l3 = [](int x) pre(({ ++x; true; })) { return x; };            // Error
  auto l4 = [c](int x) pre(({ ++c; true; })) { return x; };           // Error
  auto l5 = [] pre(({ ({ true; }); })) { return 0; };                 // OK
  return l1(v) + l2(v) + l3(v) + l4(v) + l5();
}
template <class T> T g(T t) {
  auto l = [](T x) pre(({ x > 0; })) { return x; };                   // OK
  return l(t);
}
int h(int v) {
  auto l = [](auto x) pre(({ x > 0; })) { return x; };                // OK
  return l(v);
}
int k(int x) pre(({ x > 0; }));                                        // Error
void m() {
  struct S { int n(int x) pre(({ x > 0; })) { return x; } };           // Error
}
