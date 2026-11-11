//remark:contracts: a lambda's result name cannot have the name of one of its init-captures
//type:fn
//match_regex:line 12: error: the result name of a postcondition cannot have the same name as an init-capture of the lambda
//match_regex:line 13: error: the result name of a postcondition cannot have the same name as an init-capture of the lambda
//match_regex:line 14: error: the result name of a postcondition cannot have the same name as an init-capture of the lambda
//match_regex:line 17: error: the result name of a postcondition cannot have the same name as an init-capture of the lambda
//match_regex:line 21: error: the result name of a postcondition cannot have the same name as an init-capture of the lambda
// [basic.scope.contract]/2: on a lambda-declarator, the result name cannot
// have the name of a declaration whose target scope is the nearest enclosing
// lambda scope, as an init-capture's is (EDG-89).
int v = 0;
auto a = [x = 1] (int y) post (x: x > 0) { return y; };        // Error
auto b = [&r = v] (int y) post (r: r > 0) { return y; };       // Error
auto c = [x = 1] (auto y) post (x: x > 0) { return y; };       // Error
int ci = c(1);
template <class... T> int pack(T... t) {
  return [...xs = t] (int y) post (xs: xs > 0) { return y; }(1);  // Error
}
int pi = pack(1, 2);
template <class T> T in_template(T t) {
  return [x = t] (T y) post (x: x > 0) { return y; }(t);        // Error
}
int ti = in_template(1);
// A simple capture declares nothing; "_" is name-independent (P2169).
int s = [_ = 0] (int y) post (_: true) { return y; }(1);       // OK
int w = [] (int y) { int x = y; return [x] (int z) post (x: x > 0) {
          return z; }(x); }(1);                                 // OK
// The nearest enclosing lambda scope of the inner postcondition is the inner
// lambda's, and a function declared in the body has its own parameter scope.
int n = [x = 1] {                                               // OK
  return [] (int y) post (x: x > 0) { return y; }(x);
}();
int l = [x = 1] {                                               // OK
  int f(int) post (x: x > 0);
  return f(x);
}();
