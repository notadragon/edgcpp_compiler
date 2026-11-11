//remark:contracts: a lambda in a precondition or postcondition modifies only the copies of a mutable lambda; one capturing a value parameter in a postcondition needs it const; implicit captures need a capture-default
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 18: error: expression must be a modifiable lvalue
//match_regex:line 19: error: expression must be a modifiable lvalue
//match_regex:line 21: error: expression must be a modifiable lvalue
//match_regex:line 22: error: expression must be a modifiable lvalue
//match_regex:line 24: error: expression must be a modifiable lvalue
//match_regex:line 25: error: expression must be a modifiable lvalue
//match_regex:line 27: error: value parameter "x" used in a postcondition must be declared const
//match_regex:line 28: error: value parameter "x" used in a postcondition must be declared const
//match_regex:line 31: error: an enclosing-function local variable cannot be referenced
//match_regex:line 32: error: an enclosing-function local variable cannot be referenced
//match_regex:line 35: error: in a precondition of a constructor or a postcondition of a destructor
//match_regex:line 37: error: in a precondition of a constructor or a postcondition of a destructor
struct S {
  int m;
  void f() pre([this] { return ++m > 0; }());                 // Error
  void g() pre([&] { return ++m > 0; }());                    // Error
};
void a(int x) pre([&] { return ++x > 0; }());                 // Error
void b(int x) pre([&x] { return ++x > 0; }());                // Error
void c(int x) pre([x]() mutable { return ++x > 0; }());       // OK, a copy
void d(int x) pre([x] { return ++x > 0; }());                 // Error
int e() post(r: [&r] { return ++r > 0; }());                  // Error
int e2() post(r: [r]() mutable { return ++r > 0; }());        // OK, a copy
int p(int x) post([&] { return x > 0; }());                   // Error
int p2(int x) post([x] { return x > 0; }());                  // Error
int p3(int x) post([&] { return sizeof(x) > 0; }());          // OK, unevaluated
int p4(const int x) post([&] { return x > 0; }());            // OK
void n(int x) pre([] { return x > 0; }());                    // Error
void n2(int x) pre([=] { return [] { return x > 0; }(); }()); // Error
struct C {
  int m;
  C(int) pre([&] { return m > 0; }());                       // Error
  C(long) pre([this] { return this->m > 0; }());            // OK
  ~C() post([&] { return m > 0; }());                        // Error
};
